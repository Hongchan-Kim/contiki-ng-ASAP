#include "contiki.h"
#include "net/routing/routing.h"
#include "lib/random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

/* to reset log */
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/sicslowpan.h"
#include "net/mac/tsch/tsch.h"
#include "net/routing/rpl-classic/rpl.h"
#include "services/simple-energest/simple-energest.h"
#include "orchestra.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP // LOG_LEVEL_INFO

#include "node-info.h"

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

#ifndef APP_PRINT_NODE_INFO_DELAY
#define APP_PRINT_NODE_INFO_DELAY   (1 * 30 * CLOCK_SECOND)
#endif

#ifndef APP_RESET_LOG_DELAY
#define APP_RESET_LOG_DELAY         (25 * 60 * CLOCK_SECOND)
#endif

#ifndef APP_DATA_START_DELAY
#define APP_DATA_START_DELAY        (30 * 60 * CLOCK_SECOND)
#endif

#ifndef APP_DOWNWARD_SEND_INTERVAL
#define APP_DOWNWARD_SEND_INTERVAL  (1 * 60 * CLOCK_SECOND)
#endif

#ifndef APP_DOWNWARD_MAX_TX
#define APP_DOWNWARD_MAX_TX          100
#endif

#if WITH_DOWNWARD_TRAFFIC
#define APP_DOWN_INTERVAL   (APP_DOWNWARD_SEND_INTERVAL / (NON_ROOT_NUM + 1))

static unsigned count = 1;
#endif

static struct simple_udp_connection udp_conn;

/*---------------------------------------------------------------------------*/
#if APP_SEQNO_DUPLICATE_CHECK
struct app_up_seqno {
  clock_time_t app_up_timestamp;
  uint16_t app_up_seqno;
};
struct app_up_seqnos_from_sender {
  struct app_up_seqno app_up_seqno_array[APP_SEQNO_HISTORY];
};
static struct app_up_seqnos_from_sender app_up_received_seqnos[NODE_NUM];
/*---------------------------------------------------------------------------*/
int
app_up_sequence_is_duplicate(uint16_t sender_id, uint16_t current_app_up_seqno)
{
  int i;
  clock_time_t now = clock_time();

  /*
   * Check for duplicate packet by comparing the sequence number of the incoming
   * packet with the last few ones we saw.
   */
  uint16_t sender_index = sender_id - 1;

  for(i = 0; i < APP_SEQNO_HISTORY; ++i) {
    if(current_app_up_seqno == app_up_received_seqnos[sender_index].app_up_seqno_array[i].app_up_seqno) {
#if APP_SEQNO_MAX_AGE > 0
      if(now - app_up_received_seqnos[sender_index].app_up_seqno_array[i].app_up_timestamp <= APP_SEQNO_MAX_AGE) {
        /* Duplicate packet. */
        return 1;
      }
      break;
#else /* APP_SEQNO_MAX_AGE > 0 */
      return 1;
#endif /* APP_SEQNO_MAX_AGE > 0 */
    }
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
void
app_up_sequence_register_seqno(uint16_t sender_id, uint16_t current_app_up_seqno)
{
  int i, j;

  uint16_t sender_index = sender_id - 1;

  /* Locate possible previous sequence number for this address. */
  for(i = 0; i < APP_SEQNO_HISTORY; ++i) {
    if(current_app_up_seqno == app_up_received_seqnos[sender_index].app_up_seqno_array[i].app_up_seqno) {
      i++;
      break;
    }
  }

  /* Keep the last sequence number for each address as per 802.15.4e. */
  for(j = i - 1; j > 0; --j) {
    memcpy(&app_up_received_seqnos[sender_index].app_up_seqno_array[j], &app_up_received_seqnos[sender_index].app_up_seqno_array[j - 1], sizeof(struct app_up_seqno));
  }
  app_up_received_seqnos[sender_index].app_up_seqno_array[0].app_up_seqno = current_app_up_seqno;
  app_up_received_seqnos[sender_index].app_up_seqno_array[0].app_up_timestamp = clock_time();
}
#endif /* APP_SEQNO_DUPLICATE_CHECK */
/*---------------------------------------------------------------------------*/
static void
reset_log_app_server()
{
  uint8_t i = 0;
  for(i = 0; i < NODE_NUM; i++) {
    iotlab_nodes[i][2] = 0;

#if APP_SEQNO_DUPLICATE_CHECK
    uint8_t j = 0;
    for(j = 0; j < APP_SEQNO_HISTORY; j++) {
      app_up_received_seqnos[i].app_up_seqno_array[j].app_up_seqno = 0;
      app_up_received_seqnos[i].app_up_seqno_array[j].app_up_timestamp = 0;
    }
#endif
  }

#if WITH_DOWNWARD_TRAFFIC
  count = 1;
#endif
}
/*---------------------------------------------------------------------------*/
static void
reset_log()
{
#if HCK_RPL_FIXED_TOPOLOGY || APP_TOPOLOGY_OPT_DURING_BOOTSTRAP
  tsch_queue_reset_except_n_eb();
  tsch_queue_free_unused_neighbors();
#endif
  reset_log_app_server();         /* udp-server.c */
  reset_log_tcpip();              /* tcpip.c */
  reset_log_sicslowpan();         /* sicslowpan.c */
  reset_log_tsch();               /* tsch.c */
  reset_log_rpl_icmp6();          /* rpl-icmp6.c */
  reset_log_rpl_dag();            /* rpl-dag.c */
  reset_log_rpl_timers();         /* rpl-timers.c */
  reset_log_rpl();                /* rpl.c */
  reset_log_rpl_ext_header();     /* rpl-ext-header.c */
  reset_log_simple_energest();    /* simple-energest.c */

#if HCK_ORCHESTRA_PACKET_DROP_DURING_BOOTSTRAP
  reset_flag_alice();
#endif

  uint64_t app_server_reset_log_asn = tsch_calculate_current_asn();
  LOG_HK("reset_log 1 rs_opku %u rs_q %d | rs_q_eb %d at %llx\n", 
        orchestra_parent_knows_us,
        tsch_queue_global_packet_count(),
        tsch_queue_nbr_packet_count(n_eb),
        app_server_reset_log_asn);

#if HCK_LOG_EVAL_CONFIG
  LOG_HK("eval_config 1 fixed_topology %u lite_log %u |\n", 
          HCK_RPL_FIXED_TOPOLOGY, HCK_LOG_LEVEL_LITE);
  LOG_HK("eval_config 2 traffic_load %u down_traffic_load %u app_payload_len %u |\n", 
          WITH_UPWARD_TRAFFIC ? (60 * CLOCK_SECOND / APP_UPWARD_SEND_INTERVAL) : 0, 
          WITH_DOWNWARD_TRAFFIC ? (60 * CLOCK_SECOND / APP_DOWNWARD_SEND_INTERVAL) : 0,
          APP_PAYLOAD_LEN);
  LOG_HK("eval_config 3 slot_len %u ucsf_period %u |\n", 
          HCK_TSCH_TIMESLOT_LENGTH, ORCHESTRA_CONF_UNICAST_PERIOD);
#if WITH_UPA
  LOG_HK("eval_config 4 with_upa %u |\n", WITH_UPA);
#endif
#if WITH_SLA
  LOG_HK("eval_config 5 with_sla %u sla_k %u |\n", WITH_SLA, SLA_K_TH_PERCENTILE);
#endif
#if WITH_TSCH_DEFAULT_BURST_TRANSMISSION
  LOG_HK("eval_config 6 with_dbt %u |\n", WITH_TSCH_DEFAULT_BURST_TRANSMISSION);
#endif
#if WITH_A3
  LOG_HK("eval_config 7 with_a3 %u a3_max_zone %u |\n", WITH_A3, A3_MAX_ZONE);
#endif
#endif
}
/*---------------------------------------------------------------------------*/
static void
print_log()
{
  print_log_tsch();
  print_log_rpl_timers();
  print_log_rpl();
  print_log_simple_energest();
}
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  uint64_t app_rx_up_asn = tsch_calculate_current_asn();

  uint64_t app_tx_up_asn = 0;
  memcpy(&app_tx_up_asn, data + datalen - 14, 8);

  uint16_t sender_id = ((sender_addr->u8[14]) << 8) + sender_addr->u8[15];
  uint16_t sender_index = sender_id - 1;

  if(sender_index == NODE_NUM) {
    LOG_HK("| Fail to receive: out of index\n");
    return;
  }

  uint32_t app_received_seqno = 0;
  memcpy(&app_received_seqno, data + datalen - 6, 4);

#if APP_SEQNO_DUPLICATE_CHECK
  uint16_t app_received_seqno_count = app_received_seqno % (1 << 16);

  if(app_up_sequence_is_duplicate(sender_id, app_received_seqno_count)) {
    LOG_HK("| dup_up from %u a_seq %lx at %llx\n", 
              sender_id,
              app_received_seqno,
              app_rx_up_asn);
  } else {
    app_up_sequence_register_seqno(sender_id, app_received_seqno_count);
#endif /* APP_SEQNO_DUPLICATE_CHECK */
    LOG_INFO("Received message from ");
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_("\n");

    ++iotlab_nodes[sender_index][2];
    uint8_t hops = uip_ds6_if.cur_hop_limit - UIP_IP_BUF->ttl + 1;

    LOG_HK("rx_up %u | from %u a_seq %lx len %u lt_up_r %llx lt_up_t %llx hops %u\n", 
              iotlab_nodes[sender_index][2], //
              sender_id,
              app_received_seqno,
              datalen,
              app_rx_up_asn,
              app_tx_up_asn,
              hops);
#if APP_SEQNO_DUPLICATE_CHECK
  }
#endif /* APP_SEQNO_DUPLICATE_CHECK */
}
/*---------------------------------------------------------------------------*/
PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  static struct etimer print_node_info_timer;
  static struct etimer reset_log_timer;
  static struct etimer print_log_timer;

#if WITH_DOWNWARD_TRAFFIC
  static struct etimer data_start_timer;
  static struct etimer data_periodic_timer;
  static struct etimer data_send_timer;

  static unsigned count = 1;
  uip_ipaddr_t dest_ipaddr;
  static unsigned dest_id = APP_ROOT_ID + 1;

  static uint8_t app_payload[128];
  static uint16_t current_payload_len = APP_PAYLOAD_LEN;
  static uint32_t app_seqno = 0;
  static uint16_t app_magic = (uint16_t)APP_DATA_MAGIC;

#if WITH_VARYING_PPM
  static int APP_DOWNWARD_SEND_VARYING_PPM[VARY_LENGTH] = {1, 8, 2, 4, 6, 8, 4, 1};
  static int APP_DOWNWARD_SEND_VARYING_INTERVAL[VARY_LENGTH];
  static int APP_DOWN_VARYING_INTERVAL[VARY_LENGTH];
  static int APP_DOWNWARD_VARYING_MAX_TX[VARY_LENGTH];

  for(int k = 0; k < VARY_LENGTH; k++) {
    APP_DOWNWARD_SEND_VARYING_INTERVAL[k] = (1 * 60 * CLOCK_SECOND / APP_DOWNWARD_SEND_VARYING_PPM[k]);
    APP_DOWNWARD_VARYING_MAX_TX[k] = (APP_DATA_PERIOD / VARY_LENGTH) / APP_DOWNWARD_SEND_VARYING_INTERVAL[k]);
    APP_DOWN_VARYING_INTERVAL[k] = APP_DOWNWARD_SEND_VARYING_INTERVAL[k] / (NON_ROOT_NUM + 1);
  }

  static unsigned varycount = 1;
  static int index = 0;
#endif
#endif

  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  etimer_set(&print_node_info_timer, APP_PRINT_NODE_INFO_DELAY);
  etimer_set(&reset_log_timer, APP_RESET_LOG_DELAY);
  etimer_set(&print_log_timer, APP_PRINT_LOG_DELAY);

#if WITH_DOWNWARD_TRAFFIC
#if WITH_VARYING_PPM
  etimer_set(&data_start_timer, (APP_DATA_START_DELAY + random_rand() % (APP_DOWNWARD_SEND_VARYING_INTERVAL[0] / 2)));
#else
  etimer_set(&data_start_timer, (APP_DATA_START_DELAY + random_rand() % (APP_DOWNWARD_SEND_INTERVAL / 2)));
#endif
#endif

#if HCK_LOG_EVAL_CONFIG
  LOG_HK("eval_config 1 fixed_topology %u lite_log %u |\n", 
          HCK_RPL_FIXED_TOPOLOGY, HCK_LOG_LEVEL_LITE);
  LOG_HK("eval_config 2 traffic_load %u down_traffic_load %u app_payload_len %u |\n", 
          WITH_UPWARD_TRAFFIC ? (60 * CLOCK_SECOND / APP_UPWARD_SEND_INTERVAL) : 0, 
          WITH_DOWNWARD_TRAFFIC ? (60 * CLOCK_SECOND / APP_DOWNWARD_SEND_INTERVAL) : 0,
          APP_PAYLOAD_LEN);
  LOG_HK("eval_config 3 slot_len %u ucsf_period %u |\n", 
          HCK_TSCH_TIMESLOT_LENGTH, ORCHESTRA_CONF_UNICAST_PERIOD);
#if WITH_UPA
  LOG_HK("eval_config 4 with_upa %u |\n", WITH_UPA);
#endif
#if WITH_SLA
  LOG_HK("eval_config 5 with_sla %u sla_k %u |\n", WITH_SLA, SLA_K_TH_PERCENTILE);
#endif
#if WITH_TSCH_DEFAULT_BURST_TRANSMISSION
  LOG_HK("eval_config 6 with_dbt %u |\n", WITH_TSCH_DEFAULT_BURST_TRANSMISSION);
#endif
#if WITH_A3
  LOG_HK("eval_config 7 with_a3 %u a3_max_zone %u |\n", WITH_A3, A3_MAX_ZONE);
#endif
#endif

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);
    if(data == &print_node_info_timer) {
#if WITH_IOTLAB
      print_iotlab_node_info();
#endif
    } else if(data == &reset_log_timer) {
      reset_log();
    } else if(data == &print_log_timer) {
      etimer_set(&print_log_timer, APP_PRINT_LOG_PERIOD);
      print_log();
    }
#if WITH_DOWNWARD_TRAFFIC
    else if(data == &data_start_timer || data == &data_periodic_timer) {
#if WITH_VARYING_PPM
      etimer_set(&data_send_timer, APP_DOWN_VARYING_INTERVAL[index]);
      etimer_set(&data_periodic_timer, APP_DOWNWARD_SEND_VARYING_INTERVAL[index]);
#else
      etimer_set(&data_send_timer, APP_DOWN_INTERVAL);
      etimer_set(&data_periodic_timer, APP_DOWNWARD_SEND_INTERVAL);
#endif
    }
    else if(data == &data_send_timer) {
#if WITH_VARYING_PPM
      if(varycount <= APP_DOWNWARD_VARYING_MAX_TX[index]) {
        uip_ip6addr((&dest_ipaddr), 0xfd00, 0, 0, 0, 0, 0, 0, dest_id);

        uint64_t app_tx_down_asn = tsch_calculate_current_asn();

        app_seqno = (2 << 28) + ((uint32_t)dest_id << 16) + count;
        app_magic = (uint16_t)APP_DATA_MAGIC;

        memcpy(app_payload + current_payload_len - sizeof(app_tx_down_asn) - sizeof(app_seqno) - sizeof(app_magic), 
              &app_tx_down_asn, sizeof(app_tx_down_asn));
        memcpy(app_payload + current_payload_len - sizeof(app_seqno) - sizeof(app_magic), &app_seqno, sizeof(app_seqno));
        memcpy(app_payload + current_payload_len - sizeof(app_magic), &app_magic, sizeof(app_magic));

        /* Send to clients */
        LOG_INFO("Sending message to ");
        LOG_INFO_6ADDR(&dest_ipaddr);
        LOG_INFO_("\n");

        LOG_HK("tx_down %u | to %u a_seq %lx len %u at %llx\n", 
                  count,
                  dest_id,
                  app_seqno,
                  current_payload_len, 
                  app_tx_down_asn);

        simple_udp_sendto(&udp_conn, app_payload, current_payload_len, &dest_ipaddr);

        dest_id++;
        if(dest_id > NODE_NUM) { /* the last non-root node */
          dest_id = 2;
          varycount++;
          count++;
        } else { /* not the last non-root node yet */
          etimer_reset(&data_send_timer);
        }
      }
      else if (index < VARY_LENGTH - 1){
        index++;
        varycount = 1;
      }
#else /* WITH_VARYING_PPM */
      if(count <= APP_DOWNWARD_MAX_TX) {
        uip_ip6addr((&dest_ipaddr), 0xfd00, 0, 0, 0, 0, 0, 0, dest_id);

        uint64_t app_tx_down_asn = tsch_calculate_current_asn();

        app_seqno = (2 << 28) + ((uint32_t)dest_id << 16) + count;
        app_magic = (uint16_t)APP_DATA_MAGIC;

        memcpy(app_payload + current_payload_len - sizeof(app_tx_down_asn) - sizeof(app_seqno) - sizeof(app_magic), 
              &app_tx_down_asn, sizeof(app_tx_down_asn));
        memcpy(app_payload + current_payload_len - sizeof(app_seqno) - sizeof(app_magic), &app_seqno, sizeof(app_seqno));
        memcpy(app_payload + current_payload_len - sizeof(app_magic), &app_magic, sizeof(app_magic));

        /* Send to clients */
        LOG_INFO("Sending message to ");
        LOG_INFO_6ADDR(&dest_ipaddr);
        LOG_INFO_("\n");

        LOG_HK("tx_down %u | to %u a_seq %lx len %u at %llx\n", 
                  count,
                  dest_id,
                  app_seqno,
                  current_payload_len, 
                  app_tx_down_asn);

        simple_udp_sendto(&udp_conn, app_payload, current_payload_len, &dest_ipaddr);

        dest_id++;
        if(dest_id > NODE_NUM) { /* the last non-root node */
          dest_id = 2;
          count++;
        } else { /* not the last non-root node yet */
          etimer_reset(&data_send_timer);
        }
      }
#endif
    }
#endif
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
