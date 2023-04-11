/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         RPL timer management.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 */

/**
 * \addtogroup uip
 * @{
 */

#include "contiki.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/link-stats.h"
#include "net/ipv6/multicast/uip-mcast6.h"
#include "net/ipv6/uip-sr.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/log.h"
#include <net/mac/tsch/tsch.h>

#define LOG_MODULE "RPL"
#define LOG_LEVEL LOG_LEVEL_RPL

/*---------------------------------------------------------------------------*/
static uint16_t rpl_dio_reset_count;
static uint16_t first_hop_distance_measure;
static uint16_t next_hop_distance_measure;
static uint32_t hop_distance_measure_count;
static uint32_t hop_distance_measure_sum;
#if HCK_RPL_IGNORE_REDUNDANCY_IN_BOOTSTRAP
static uint8_t ignore_redundancy = 1;
#endif
/*---------------------------------------------------------------------------*/
void print_log_rpl_timers()
{
  rpl_dag_t *dag = rpl_get_any_dag();
  LOG_HK("hopD_now %u hopD_sum %lu hopD_cnt %lu |\n", 
        dag->rank == ROOT_RANK(dag->instance) ? 
          0 : dag->preferred_parent->hop_distance + 1, 
        hop_distance_measure_sum, hop_distance_measure_count);
}
/*---------------------------------------------------------------------------*/
void reset_log_rpl_timers()
{
  print_log_rpl_timers();
#if HCK_RPL_IGNORE_REDUNDANCY_IN_BOOTSTRAP
  ignore_redundancy = 0;
#endif
  rpl_dio_reset_count = 0;
  first_hop_distance_measure = 0;
  next_hop_distance_measure = 0;
  hop_distance_measure_count = 0;
  hop_distance_measure_sum = 0;
}
/*---------------------------------------------------------------------------*/

/* A configurable function called after update of the RPL DIO interval */
#ifdef RPL_CALLBACK_NEW_DIO_INTERVAL
void RPL_CALLBACK_NEW_DIO_INTERVAL(clock_time_t dio_interval);
#endif /* RPL_CALLBACK_NEW_DIO_INTERVAL */

#ifdef RPL_PROBING_SELECT_FUNC
rpl_parent_t *RPL_PROBING_SELECT_FUNC(rpl_dag_t *dag);
#endif /* RPL_PROBING_SELECT_FUNC */

#ifdef RPL_PROBING_DELAY_FUNC
clock_time_t RPL_PROBING_DELAY_FUNC(rpl_dag_t *dag);
#endif /* RPL_PROBING_DELAY_FUNC */

/*---------------------------------------------------------------------------*/
static struct ctimer periodic_timer;

static void handle_periodic_timer(void *ptr);
static void new_dio_interval(rpl_instance_t *instance);
static void handle_dio_timer(void *ptr);

static uint16_t next_dis;

/* dio_send_ok is true if the node is ready to send DIOs */
static uint8_t dio_send_ok;

/*---------------------------------------------------------------------------*/
static void
handle_periodic_timer(void *ptr)
{
  rpl_dag_t *dag = rpl_get_any_dag();

  rpl_purge_dags();
  if(dag != NULL) {
    if(RPL_IS_STORING(dag->instance)) {
      rpl_purge_routes();
    }
    if(RPL_IS_NON_STORING(dag->instance)) {
      uip_sr_periodic(1);
    }
  }
  rpl_recalculate_ranks();

  if(first_hop_distance_measure < RPL_FIRST_MEASURE_PERIOD) {
    first_hop_distance_measure++;
  } else {
    next_hop_distance_measure++;
    if(next_hop_distance_measure >= RPL_NEXT_MEASURE_PERIOD) {

      if((tsch_is_associated == 1) && (dag->preferred_parent != NULL) 
        && (dag->preferred_parent->rank != RPL_INFINITE_RANK) 
        && (dag->preferred_parent->hop_distance != 0xff)) {
        uint8_t my_hop_distance = dag->preferred_parent->hop_distance + 1;
        hop_distance_measure_sum += (uint32_t)my_hop_distance;
        hop_distance_measure_count++;
      }

      next_hop_distance_measure = 0;
    }
  }

  /* handle DIS */
#if RPL_DIS_SEND
  next_dis++;
  if((dag == NULL || dag->instance->current_dag->rank == RPL_INFINITE_RANK) && next_dis >= RPL_DIS_INTERVAL) {
    next_dis = 0;
    dis_output(NULL);
  }
#endif
  ctimer_reset(&periodic_timer);
}
/*---------------------------------------------------------------------------*/
static void
new_dio_interval(rpl_instance_t *instance)
{
  uint32_t time;
  clock_time_t ticks;

  /* TODO: too small timer intervals for many cases */
  time = 1UL << instance->dio_intcurrent;

  /* Convert from milliseconds to CLOCK_TICKS. */
  ticks = (time * CLOCK_SECOND) / 1000;
  instance->dio_next_delay = ticks;

  /* random number between I/2 and I */
  ticks = ticks / 2 + (ticks / 2 * (uint32_t)random_rand()) / RANDOM_RAND_MAX;

  /*
   * The intervals must be equally long among the nodes for Trickle to
   * operate efficiently. Therefore we need to calculate the delay between
   * the randomized time and the start time of the next interval.
   */
  instance->dio_next_delay -= ticks;
  instance->dio_send = 1;

#if RPL_CONF_STATS
  /* keep some stats */
  instance->dio_totint++;
  instance->dio_totrecv += instance->dio_counter;
  LOG_ANNOTATE("#A rank=%u.%u(%u),stats=%d %d %d %d,color=%s\n",
	   DAG_RANK(instance->current_dag->rank, instance),
           (10 * (instance->current_dag->rank % instance->min_hoprankinc)) / instance->min_hoprankinc,
           instance->current_dag->version,
           instance->dio_totint, instance->dio_totsend,
           instance->dio_totrecv,instance->dio_intcurrent,
	   instance->current_dag->rank == ROOT_RANK(instance) ? "BLUE" : "ORANGE");
#endif /* RPL_CONF_STATS */

  /* reset the redundancy counter */
  instance->dio_counter = 0;

  /* schedule the timer */
  LOG_INFO("Scheduling DIO timer %lu ticks in future (Interval)\n", ticks);
  ctimer_set(&instance->dio_timer, ticks, &handle_dio_timer, instance);

#ifdef RPL_CALLBACK_NEW_DIO_INTERVAL
  RPL_CALLBACK_NEW_DIO_INTERVAL((CLOCK_SECOND * 1UL << instance->dio_intcurrent) / 1000);
#endif /* RPL_CALLBACK_NEW_DIO_INTERVAL */
}
/*---------------------------------------------------------------------------*/
static void
handle_dio_timer(void *ptr)
{
  rpl_instance_t *instance;

  instance = (rpl_instance_t *)ptr;

  LOG_DBG("DIO Timer triggered\n");
  if(!dio_send_ok) {
    if(uip_ds6_get_link_local(ADDR_PREFERRED) != NULL) {
      dio_send_ok = 1;
    } else {
      LOG_WARN("Postponing DIO transmission since link local address is not ok\n");
      ctimer_set(&instance->dio_timer, CLOCK_SECOND, &handle_dio_timer, instance);
      return;
    }
  }

  if(instance->dio_send) {
    /* send DIO if counter is less than desired redundancy */
#if HCK_RPL_IGNORE_REDUNDANCY_IN_BOOTSTRAP
    if(ignore_redundancy || instance->dio_redundancy == 0 || instance->dio_counter < instance->dio_redundancy) {
#else
    if(instance->dio_redundancy == 0 || instance->dio_counter < instance->dio_redundancy) {
#endif
#if RPL_CONF_STATS
      instance->dio_totsend++;
#endif /* RPL_CONF_STATS */
      dio_output(instance, NULL);
    } else {
      LOG_DBG("Suppressing DIO transmission (%d >= %d)\n",
             instance->dio_counter, instance->dio_redundancy);
    }
    instance->dio_send = 0;
    LOG_DBG("Scheduling DIO timer %lu ticks in future (sent)\n",
           instance->dio_next_delay);
    ctimer_set(&instance->dio_timer, instance->dio_next_delay, handle_dio_timer, instance);
  } else {
    /* check if we need to double interval */
    if(instance->dio_intcurrent < instance->dio_intmin + instance->dio_intdoubl) {
      instance->dio_intcurrent++;
      LOG_DBG("DIO Timer interval doubled %d\n", instance->dio_intcurrent);
    }
    new_dio_interval(instance);
  }

  if(LOG_DBG_ENABLED) {
    rpl_print_neighbor_list();
  }
}
/*---------------------------------------------------------------------------*/
void
rpl_reset_periodic_timer(void)
{
  next_dis = RPL_DIS_INTERVAL / 2 +
    ((uint32_t)RPL_DIS_INTERVAL * (uint32_t)random_rand()) / RANDOM_RAND_MAX -
    RPL_DIS_START_DELAY;
  ctimer_set(&periodic_timer, CLOCK_SECOND, handle_periodic_timer, NULL);
}
/*---------------------------------------------------------------------------*/
/* Resets the DIO timer in the instance to its minimal interval. */
void
rpl_reset_dio_timer(rpl_instance_t *instance)
{
#if !RPL_LEAF_ONLY
  /* Do not reset if we are already on the minimum interval,
     unless forced to do so. */
  ++rpl_dio_reset_count;
  LOG_HK("rdt %u | reset DIO timer\n", rpl_dio_reset_count);

  if(instance->dio_intcurrent > instance->dio_intmin) {
    instance->dio_counter = 0;
    instance->dio_intcurrent = instance->dio_intmin;
    new_dio_interval(instance);
  }
#if RPL_CONF_STATS
  rpl_stats.resets++;
#endif /* RPL_CONF_STATS */
#endif /* RPL_LEAF_ONLY */
}
/*---------------------------------------------------------------------------*/
static void handle_dao_timer(void *ptr);
static void
set_dao_lifetime_timer(rpl_instance_t *instance)
{
  if(rpl_get_mode() == RPL_MODE_FEATHER) {
    return;
  }

  /* Set up another DAO within half the expiration time, if such a
     time has been configured */
  if(instance->default_lifetime != RPL_INFINITE_LIFETIME) {
    clock_time_t expiration_time;
    expiration_time = (clock_time_t)instance->default_lifetime *
      (clock_time_t)instance->lifetime_unit *
      CLOCK_SECOND / 2;
    /* make the time for the re registration be betwen 1/2 - 3/4 of lifetime */
    expiration_time = expiration_time + (random_rand() % (expiration_time / 2));
    LOG_DBG("Scheduling DAO lifetime timer %u ticks in the future\n",
           (unsigned)expiration_time);
    ctimer_set(&instance->dao_lifetime_timer, expiration_time,
               handle_dao_timer, instance);
  }
}
/*---------------------------------------------------------------------------*/
static void
handle_dao_timer(void *ptr)
{
  rpl_instance_t *instance;
#if RPL_WITH_MULTICAST
  uip_mcast6_route_t *mcast_route;
  uint8_t i;
#endif

  instance = (rpl_instance_t *)ptr;

  if(!dio_send_ok && uip_ds6_get_link_local(ADDR_PREFERRED) == NULL) {
    LOG_INFO("Postpone DAO transmission\n");
    ctimer_set(&instance->dao_timer, CLOCK_SECOND, handle_dao_timer, instance);
    return;
  }

  /* Send the DAO to the DAO parent set -- the preferred parent in our case. */
  if(instance->current_dag->preferred_parent != NULL) {
    LOG_INFO("handle_dao_timer - sending DAO\n");
    /* Set the route lifetime to the default value. */
    dao_output(instance->current_dag->preferred_parent, instance->default_lifetime);

#if RPL_WITH_MULTICAST
    /* Send DAOs for multicast prefixes only if the instance is in MOP 3 */
    if(instance->mop == RPL_MOP_STORING_MULTICAST) {
      /* Send a DAO for own multicast addresses */
      for(i = 0; i < UIP_DS6_MADDR_NB; i++) {
        if(uip_ds6_if.maddr_list[i].isused
            && uip_is_addr_mcast_global(&uip_ds6_if.maddr_list[i].ipaddr)) {
          dao_output_target(instance->current_dag->preferred_parent,
              &uip_ds6_if.maddr_list[i].ipaddr, instance->default_lifetime);
        }
      }

      /* Iterate over multicast routes and send DAOs */
      mcast_route = uip_mcast6_route_list_head();
      while(mcast_route != NULL) {
        /* Don't send if it's also our own address, done that already */
        if(uip_ds6_maddr_lookup(&mcast_route->group) == NULL) {
          dao_output_target(instance->current_dag->preferred_parent,
                     &mcast_route->group, instance->default_lifetime);
        }
        mcast_route = list_item_next(mcast_route);
      }
    }
#endif
  } else {
    LOG_INFO("No suitable DAO parent\n");
  }

  ctimer_stop(&instance->dao_timer);

  if(etimer_expired(&instance->dao_lifetime_timer.etimer)) {
    set_dao_lifetime_timer(instance);
  }
}
/*---------------------------------------------------------------------------*/
static void
schedule_dao(rpl_instance_t *instance, clock_time_t latency)
{
  clock_time_t expiration_time;

  if(rpl_get_mode() == RPL_MODE_FEATHER) {
    return;
  }

  expiration_time = etimer_expiration_time(&instance->dao_timer.etimer);

  if(!etimer_expired(&instance->dao_timer.etimer)) {
    LOG_DBG("DAO timer already scheduled\n");
  } else {
    if(latency != 0) {
      expiration_time = latency / 2 +
        (random_rand() % (latency));
    } else {
      expiration_time = 0;
    }
    LOG_DBG("Scheduling DAO timer %u ticks in the future\n",
           (unsigned)expiration_time);
    ctimer_set(&instance->dao_timer, expiration_time,
               handle_dao_timer, instance);

    set_dao_lifetime_timer(instance);
  }
}
/*---------------------------------------------------------------------------*/
void
rpl_schedule_dao(rpl_instance_t *instance)
{
  schedule_dao(instance, RPL_DAO_DELAY);
}
/*---------------------------------------------------------------------------*/
void
rpl_schedule_dao_immediately(rpl_instance_t *instance)
{
  schedule_dao(instance, 0);
}
/*---------------------------------------------------------------------------*/
void
rpl_cancel_dao(rpl_instance_t *instance)
{
  ctimer_stop(&instance->dao_timer);
  ctimer_stop(&instance->dao_lifetime_timer);
}
/*---------------------------------------------------------------------------*/
static void
handle_unicast_dio_timer(void *ptr)
{
  rpl_instance_t *instance = (rpl_instance_t *)ptr;
  uip_ipaddr_t *target_ipaddr = rpl_parent_get_ipaddr(instance->unicast_dio_target);

  if(target_ipaddr != NULL) {
    dio_output(instance, target_ipaddr);
  }
}
/*---------------------------------------------------------------------------*/
void
rpl_schedule_unicast_dio_immediately(rpl_instance_t *instance)
{
  ctimer_set(&instance->unicast_dio_timer, 0,
                  handle_unicast_dio_timer, instance);
}
/*---------------------------------------------------------------------------*/
#if RPL_WITH_PROBING
clock_time_t
get_probing_delay(rpl_dag_t *dag)
{
  return ((RPL_PROBING_INTERVAL) / 2) + random_rand() % (RPL_PROBING_INTERVAL);
}
/*---------------------------------------------------------------------------*/
rpl_parent_t *
get_probing_target(rpl_dag_t *dag)
{
  /* Returns the next probing target. The current implementation probes the urgent
   * probing target if any, or the preferred parent if its link statistics need refresh.
   * Otherwise, it picks at random between:
   * (1) selecting the best parent with non-fresh link statistics
   * (2) selecting the least recently updated parent
   */

  rpl_parent_t *p;
  rpl_parent_t *probing_target = NULL;
  rpl_rank_t probing_target_rank = RPL_INFINITE_RANK;
  clock_time_t probing_target_age = 0;
  clock_time_t clock_now = clock_time();

  if(dag == NULL ||
      dag->instance == NULL) {
    return NULL;
  }

  /* There is an urgent probing target */
  if(dag->instance->urgent_probing_target != NULL) {
    return dag->instance->urgent_probing_target;
  }

  /* The preferred parent needs probing */
  if(dag->preferred_parent != NULL && !rpl_parent_is_fresh(dag->preferred_parent)) {
    return dag->preferred_parent;
  }

  /* With 50% probability: probe best non-fresh parent */
  if(random_rand() % 2 == 0) {
    p = nbr_table_head(rpl_parents);
    while(p != NULL) {
      if(p->dag == dag && !rpl_parent_is_fresh(p)) {
        /* p is in our dag and needs probing */
        rpl_rank_t p_rank = rpl_rank_via_parent(p);
        if(probing_target == NULL
            || p_rank < probing_target_rank) {
          probing_target = p;
          probing_target_rank = p_rank;
        }
      }
      p = nbr_table_next(rpl_parents, p);
    }
  }

  /* If we still do not have a probing target: pick the least recently updated parent */
  if(probing_target == NULL) {
    p = nbr_table_head(rpl_parents);
    while(p != NULL) {
      const struct link_stats *stats =rpl_get_parent_link_stats(p);
      if(p->dag == dag && stats != NULL) {
        if(probing_target == NULL
            || clock_now - stats->last_tx_time > probing_target_age) {
          probing_target = p;
          probing_target_age = clock_now - stats->last_tx_time;
        }
      }
      p = nbr_table_next(rpl_parents, p);
    }
  }

  return probing_target;
}
/*---------------------------------------------------------------------------*/
static rpl_dag_t *
get_next_dag(rpl_instance_t *instance)
{
  rpl_dag_t *dag = NULL;
  int new_dag = instance->last_dag;
  do {
    new_dag++;
    if(new_dag >= RPL_MAX_DAG_PER_INSTANCE) {
      new_dag = 0;
    }
    if(instance->dag_table[new_dag].used) {
      dag = &instance->dag_table[new_dag];
    }
  } while(new_dag != instance->last_dag && dag == NULL);
  instance->last_dag = new_dag;
  return dag;
}
/*---------------------------------------------------------------------------*/
static void
handle_probing_timer(void *ptr)
{
  rpl_instance_t *instance = (rpl_instance_t *)ptr;
  rpl_parent_t *probing_target = RPL_PROBING_SELECT_FUNC(get_next_dag(instance));
  uip_ipaddr_t *target_ipaddr = rpl_parent_get_ipaddr(probing_target);

  /* Perform probing */
  if(target_ipaddr != NULL) {
    const struct link_stats *stats = rpl_get_parent_link_stats(probing_target);
    const linkaddr_t *lladdr = rpl_get_parent_lladdr(probing_target);
    LOG_INFO("probing %u %s last tx %u min ago\n",
          lladdr != NULL ? lladdr->u8[7] : 0x0,
          instance->urgent_probing_target != NULL ? "(urgent)" : "",
          probing_target != NULL && stats != NULL ?
           (unsigned)((clock_time() - stats->last_tx_time) / (60 * CLOCK_SECOND)) : 0
      );

    /* Send probe, e.g. unicast DIO or DIS */
    RPL_PROBING_SEND_FUNC(instance, target_ipaddr);
  }

  /* Schedule next probing */
  rpl_schedule_probing(instance);

  if(LOG_DBG_ENABLED) {
    rpl_print_neighbor_list();
  }
}
/*---------------------------------------------------------------------------*/
void
rpl_schedule_probing(rpl_instance_t *instance)
{
  ctimer_set(&instance->probing_timer, RPL_PROBING_DELAY_FUNC(instance->current_dag),
                  handle_probing_timer, instance);
}
/*---------------------------------------------------------------------------*/
void
rpl_schedule_probing_now(rpl_instance_t *instance)
{
  ctimer_set(&instance->probing_timer, random_rand() % (CLOCK_SECOND * 4),
                  handle_probing_timer, instance);
}
#endif /* RPL_WITH_PROBING */
/** @}*/
