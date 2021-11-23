#include "node-info.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#if WITH_IOTLAB

#if IOTLAB_SITE == IOTLAB_GRENOBLE_83_CORNER
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0},
  {287, 0xb982, 0},
  {286, 0xa969, 0},
  {285, 0xa583, 0},
  {284, 0x9875, 0},
  {283, 0xb781, 0},
  {282, 0xa283, 0},
  {281, 0xa582, 0},
  {280, 0xb381, 0},
  {279, 0x9780, 0},
  {278, 0xa968, 0},
  {277, 0xc176, 0},
  {276, 0x9482, 0},
  {275, 0x9771, 0},
  {274, 0xa880, 0},
  {273, 0x9279, 0},
  {272, 0x2861, 0},
  {271, 0xa980, 0},
  {270, 0xb380, 0},
  {269, 0xb477, 0},
  {268, 0x9681, 0},
  {267, 0xb182, 0},
  {266, 0xc369, 0},
  {265, 0xa470, 0},
  {264, 0x9575, 0},
  {263, 0x8470, 0},
  {261, 0x9483, 0},
  {260, 0xa478, 0},
  {259, 0xa580, 0},
  {258, 0x1860, 0},
  {257, 0xb668, 0},
  {256, 0x1660, 0},
  {255, 0xb168, 0},
  {254, 0x8982, 0},
  {253, 0xc277, 0},
  {252, 0xb580, 0},
  {251, 0x9876, 0},
  {250, 0xa483, 0},
  {249, 0x9583, 0},
  {248, 0xb383, 0},
  {247, 0xa982, 0},
  {246, 0xb881, 0},
  {245, 0x9269, 0},
  {244, 0x9579, 0},
  {242, 0x9077, 0},
  {241, 0x8371, 0},
  {240, 0x3761, 0},
  {239, 0xb876, 0},
  {238, 0xa083, 0},
  {237, 0x1562, 0},
  {236, 0xa677, 0},
  {235, 0x2362, 0},
  {234, 0x9481, 0},
  {233, 0x8881, 0},
  {232, 0xa372, 0},
  {231, 0x8779, 0},
  {230, 0x3861, 0},
  {229, 0xb068, 0},
  {228, 0x9283, 0},
  {227, 0x9377, 0},
  {226, 0xb268, 0},
  {225, 0xc068, 0},
  {224, 0xb180, 0},
  {223, 0xb768, 0},
  {222, 0x3860, 0},
  {221, 0x9981, 0},
  {220, 0x2160, 0},
  {219, 0xb481, 0},
  {218, 0xb582, 0},
  {217, 0x9871, 0},
  {216, 0xb369, 0},
  {215, 0x9175, 0},
  {214, 0x9567, 0},
  {213, 0x8472, 0},
  {212, 0xb280, 0},
  {211, 0x8875, 0},
  {210, 0xc081, 0},
  {209, 0xb379, 0},
  {208, 0x9076, 0},
  {207, 0xa379, 0},
  {205, 0x8967, 0},
  {204, 0xb877, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_79_CORNER
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {250, 0x2659, 0}, // root node
  {251, 0x2454, 0},
  {252, 0x2458, 0},
  {253, 0xb871, 0},
  {254, 0x2358, 0},
  {255, 0xb371, 0},
  {256, 0x2554, 0},
  {245, 0x2851, 0},
  {246, 0x2251, 0},
  {248, 0x3153, 0},
  {238, 0x9373, 0},
  {239, 0xc270, 0},
  {240, 0xb288, 0},
  {241, 0xb070, 0},
  {242, 0x2450, 0},
  {243, 0xb073, 0},
  {244, 0x1258, 0},
  {233, 0x8873, 0},
  {234, 0x1254, 0},
  {235, 0xb270, 0},
  {236, 0x2352, 0},
  {225, 0xa573, 0},
  {226, 0x9573, 0},
  {227, 0x2559, 0},
  {228, 0x1455, 0},
  {229, 0x9770, 0},
  {230, 0x2751, 0},
  {231, 0x2052, 0},
  {220, 0xa873, 0},
  {221, 0x3059, 0},
  {222, 0x9173, 0},
  {223, 0xa289, 0},
  {205, 0xb173, 0},
  {206, 0x2850, 0},
  {207, 0x3359, 0},
  {208, 0x2350, 0},
  {209, 0x2050, 0},
  {210, 0x1855, 0},
  {198, 0x1856, 0},
  {199, 0x9073, 0},
  {200, 0x9270, 0},
  {201, 0xa189, 0},
  {202, 0x9877, 0},
  {203, 0xc273, 0},
  {191, 0x8473, 0},
  {193, 0x3558, 0},
  {194, 0x1159, 0},
  {195, 0xb388, 0},
  {196, 0x2451, 0},
  {178, 0xa473, 0},
  {179, 0x660, 0},
  {180, 0x3559, 0},
  {181, 0xb172, 0},
  {182, 0x2853, 0},
  {169, 0x1459, 0},
  {170, 0xb671, 0},
  {171, 0x2258, 0},
  {172, 0x3554, 0},
  {173, 0xc170, 0},
  {174, 0x9273, 0},
  {175, 0x2459, 0},
  {161, 0xa390, 0},
  {162, 0x2854, 0},
  {163, 0x8774, 0},
  {164, 0xa077, 0},
  {165, 0x2550, 0},
  {152, 0xa173, 0},
  {153, 0xb572, 0},
  {154, 0xb071, 0},
  {155, 0xb372, 0},
  {156, 0x3759, 0},
  {157, 0x2755, 0},
  {158, 0x2154, 0},
  {143, 0x3862, 0},
  {144, 0x1759, 0},
  {145, 0xb373, 0},
  {146, 0xb189, 0},
  {147, 0x2151, 0},
  {148, 0xb370, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_79_CENTER
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {208, 0x2350, 0}, // root node
  {250, 0x2659, 0},
  {251, 0x2454, 0},
  {252, 0x2458, 0},
  {253, 0xb871, 0},
  {254, 0x2358, 0},
  {255, 0xb371, 0},
  {256, 0x2554, 0},
  {245, 0x2851, 0},
  {246, 0x2251, 0},
  {248, 0x3153, 0},
  {238, 0x9373, 0},
  {239, 0xc270, 0},
  {240, 0xb288, 0},
  {241, 0xb070, 0},
  {242, 0x2450, 0},
  {243, 0xb073, 0},
  {244, 0x1258, 0},
  {233, 0x8873, 0},
  {234, 0x1254, 0},
  {235, 0xb270, 0},
  {236, 0x2352, 0},
  {225, 0xa573, 0},
  {226, 0x9573, 0},
  {227, 0x2559, 0},
  {228, 0x1455, 0},
  {229, 0x9770, 0},
  {230, 0x2751, 0},
  {231, 0x2052, 0},
  {220, 0xa873, 0},
  {221, 0x3059, 0},
  {222, 0x9173, 0},
  {223, 0xa289, 0},
  {205, 0xb173, 0},
  {206, 0x2850, 0},
  {207, 0x3359, 0},
  {209, 0x2050, 0},
  {210, 0x1855, 0},
  {198, 0x1856, 0},
  {199, 0x9073, 0},
  {200, 0x9270, 0},
  {201, 0xa189, 0},
  {202, 0x9877, 0},
  {203, 0xc273, 0},
  {191, 0x8473, 0},
  {193, 0x3558, 0},
  {194, 0x1159, 0},
  {195, 0xb388, 0},
  {196, 0x2451, 0},
  {178, 0xa473, 0},
  {179, 0x660, 0},
  {180, 0x3559, 0},
  {181, 0xb172, 0},
  {182, 0x2853, 0},
  {169, 0x1459, 0},
  {170, 0xb671, 0},
  {171, 0x2258, 0},
  {172, 0x3554, 0},
  {173, 0xc170, 0},
  {174, 0x9273, 0},
  {175, 0x2459, 0},
  {161, 0xa390, 0},
  {162, 0x2854, 0},
  {163, 0x8774, 0},
  {164, 0xa077, 0},
  {165, 0x2550, 0},
  {152, 0xa173, 0},
  {153, 0xb572, 0},
  {154, 0xb071, 0},
  {155, 0xb372, 0},
  {156, 0x3759, 0},
  {157, 0x2755, 0},
  {158, 0x2154, 0},
  {143, 0x3862, 0},
  {144, 0x1759, 0},
  {145, 0xb373, 0},
  {146, 0xb189, 0},
  {147, 0x2151, 0},
  {148, 0xb370, 0}
};
#endif /* IOTLAB_SITE */

uint16_t
iotlab_node_id_from_uid(uint16_t uid)
{
  uint16_t i = 0;
  for(i = 0; i < NODE_NUM; i++) {
    if(iotlab_nodes[i][1] == uid) {
      return i + 1;
    }
  }
  return 0; /* no matching index */
}

#endif /* WITH_IOTLAB */

/*---------------------------------------------------------------------------*/
void
print_node_info()
{
#if WITH_IOTLAB
  LOG_INFO("HCK-NODE root %u %x (%u %x)\n", IOTLAB_ROOT_ID, IOTLAB_ROOT_ID, iotlab_nodes[0][0], iotlab_nodes[0][1]);
  uint8_t i = 1;
  for(i = 1; i < NODE_NUM; i++) {
    LOG_INFO("HCK-NODE non_root %u %x (%u %x)\n", i + 1, i + 1, iotlab_nodes[i][0], iotlab_nodes[i][1]);
  }
#endif
  LOG_INFO("HCK-NODE end\n");
}
/*---------------------------------------------------------------------------*/


/* backup settings */
/*
#if IOTLAB_SITE == IOTLAB_LYON_2
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {7, 0xa371, 0}, // root node
  {3, 0x8676, 0}
};
#elif IOTLAB_SITE == IOTLAB_LYON_3
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {7, 0xa371, 0}, // root node
  {2, 0x8867, 0},
  {3, 0x8676, 0}
};
#elif IOTLAB_SITE == IOTLAB_LYON_10
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {18, 0x3261, 0}, // root node
  {1, 0x9768, 0},
  {2, 0x8867, 0},
  {3, 0x8676, 0},
  {4, 0xb181, 0},
  {5, 0x8968, 0},
  {6, 0xc279, 0},
  {7, 0xa371, 0},
  {8, 0xa683, 0},
  {10, 0x8976, 0}
};
#elif IOTLAB_SITE == IOTLAB_LYON_17
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {18, 0x3261, 0}, // root node
  {1, 0x9768, 0},
  {2, 0x8867, 0},
  {3, 0x8676, 0},
  {4, 0xb181, 0},
  {5, 0x8968, 0},
  {6, 0xc279, 0},
  {7, 0xa371, 0},
  {8, 0xa683, 0},
  {10, 0x8976, 0},
  {11, 0x8467, 0},
  {12, 0xb682, 0},
  {13, 0xb176, 0},
  {14, 0x2860, 0},
  {15, 0xa377, 0},
  {16, 0xb978, 0},
  {17, 0xa168, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_24
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {250, 0x2659, 0}, // root node
  {152, 0xa173, 0},
  {154, 0xb071, 0},
  {156, 0x3759, 0},
  {158, 0x2154, 0},
  {170, 0xb671, 0},
  {172, 0x3554, 0},
  {174, 0x9273, 0},
  {191, 0x8473, 0},
  {193, 0x3558, 0},
  {195, 0xb388, 0},
  {206, 0x2850, 0},
  {208, 0x2350, 0},
  {210, 0x1855, 0},
  {225, 0xa573, 0},
  {227, 0x2559, 0},
  {229, 0x9770, 0},
  {231, 0x2052, 0},
  {239, 0xc270, 0},
  {241, 0xb070, 0},
  {243, 0xb073, 0},
  {252, 0x2458, 0},
  {254, 0x2358, 0},
  {256, 0x2554, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_40
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {89, 0xa990, 0}, // root node
  {88, 0xc073, 0},
  {87, 0x2155, 0},
  {86, 0xb471, 0},
  {85, 0x9773, 0},
  {84, 0x9370, 0},
  {83, 0xa489, 0},
  {80, 0x2750, 0},
  {81, 0x1556, 0},
  {82, 0x2156, 0},
  {76, 0x2955, 0},
  {77, 0x1756, 0},
  {78, 0x2250, 0},
  {75, 0x8973, 0},
  {73, 0x1956, 0},
  {71, 0x1654, 0},
  {70, 0xa070, 0},
  {69, 0xa689, 0},
  {68, 0x2855, 0},
  {67, 0x1957, 0},
  {66, 0x1857, 0},
  {65, 0xb473, 0},
  {63, 0xc070, 0},
  {64, 0x1758, 0},
  {61, 0x3254, 0},
  {62, 0x2359, 0},
  {59, 0xb271, 0},
  {60, 0x761, 0},
  {54, 0x3151, 0},
  {55, 0xb771, 0},
  {56, 0xa273, 0},
  {57, 0x1256, 0},
  {58, 0x8674, 0},
  {49, 0x2159, 0},
  {51, 0xb973, 0},
  {52, 0x3658, 0},
  {53, 0x1651, 0},
  {48, 0x956, 0},
  {47, 0x1854, 0},
  {46, 0xb772, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_46
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {250, 0x2659, 0}, // root node
  {152, 0xa173, 0},
  {153, 0xb572, 0},
  {154, 0xb071, 0},
  {155, 0xb372, 0},
  {156, 0x3759, 0},
  {157, 0x2755, 0},
  {158, 0x2154, 0},
  {169, 0x1459, 0},
  {170, 0xb671, 0},
  {171, 0x2258, 0},
  {172, 0x3554, 0},
  {173, 0xc170, 0},
  {174, 0x9273, 0},
  {175, 0x2459, 0},
  {191, 0x8473, 0},
  {193, 0x3558, 0},
  {194, 0x1159, 0},
  {195, 0xb388, 0},
  {196, 0x2451, 0},
  {205, 0xb173, 0},
  {206, 0x2850, 0},
  {207, 0x3359, 0},
  {208, 0x2350, 0},
  {209, 0x2050, 0},
  {210, 0x1855, 0},
  {225, 0xa573, 0},
  {226, 0x9573, 0},
  {227, 0x2559, 0},
  {228, 0x1455, 0},
  {229, 0x9770, 0},
  {230, 0x2751, 0},
  {231, 0x2052, 0},
  {238, 0x9373, 0},
  {239, 0xc270, 0},
  {240, 0xb288, 0},
  {241, 0xb070, 0},
  {242, 0x2450, 0},
  {243, 0xb073, 0},
  {244, 0x1258, 0},
  {251, 0x2454, 0},
  {252, 0x2458, 0},
  {253, 0xb871, 0},
  {254, 0x2358, 0},
  {255, 0xb371, 0},
  {256, 0x2554, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_50
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {89, 0xa990, 0}, // root node
  {88, 0xc073, 0},
  {87, 0x2155, 0},
  {86, 0xb471, 0},
  {85, 0x9773, 0},
  {84, 0x9370, 0},
  {83, 0xa489, 0},
  {80, 0x2750, 0},
  {81, 0x1556, 0},
  {82, 0x2156, 0},
  {76, 0x2955, 0},
  {77, 0x1756, 0},
  {78, 0x2250, 0},
  {75, 0x8973, 0},
  {73, 0x1956, 0},
  {71, 0x1654, 0},
  {70, 0xa070, 0},
  {69, 0xa689, 0},
  {68, 0x2855, 0},
  {67, 0x1957, 0},
  {66, 0x1857, 0},
  {65, 0xb473, 0},
  {63, 0xc070, 0},
  {64, 0x1758, 0},
  {61, 0x3254, 0},
  {62, 0x2359, 0},
  {59, 0xb271, 0},
  {60, 0x761, 0},
  {54, 0x3151, 0},
  {55, 0xb771, 0},
  {56, 0xa273, 0},
  {57, 0x1256, 0},
  {58, 0x8674, 0},
  {49, 0x2159, 0},
  {51, 0xb973, 0},
  {52, 0x3658, 0},
  {53, 0x1651, 0},
  {48, 0x956, 0},
  {47, 0x1854, 0},
  {46, 0xb772, 0},
  {110, 0x8573, 0},
  {108, 0x1658, 0},
  {109, 0x8673, 0},
  {105, 0x3458, 0},
  {106, 0xa871, 0},
  {102, 0x1859, 0},
  {104, 0x1359, 0},
  {99, 0x959, 0},
  {100, 0x9989, 0},
  {101, 0xb870, 0}
};
#elif IOTLAB_SITE == IOTLAB_LILLE_79
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {250, 0x2659, 0}, // root node
  {251, 0x2454, 0},
  {252, 0x2458, 0},
  {253, 0xb871, 0},
  {254, 0x2358, 0},
  {255, 0xb371, 0},
  {256, 0x2554, 0},
  {245, 0x2851, 0},
  {246, 0x2251, 0},
  {248, 0x3153, 0},
  {238, 0x9373, 0},
  {239, 0xc270, 0},
  {240, 0xb288, 0},
  {241, 0xb070, 0},
  {242, 0x2450, 0},
  {243, 0xb073, 0},
  {244, 0x1258, 0},
  {233, 0x8873, 0},
  {234, 0x1254, 0},
  {235, 0xb270, 0},
  {236, 0x2352, 0},
  {225, 0xa573, 0},
  {226, 0x9573, 0},
  {227, 0x2559, 0},
  {228, 0x1455, 0},
  {229, 0x9770, 0},
  {230, 0x2751, 0},
  {231, 0x2052, 0},
  {220, 0xa873, 0},
  {221, 0x3059, 0},
  {222, 0x9173, 0},
  {223, 0xa289, 0},
  {205, 0xb173, 0},
  {206, 0x2850, 0},
  {207, 0x3359, 0},
  {208, 0x2350, 0},
  {209, 0x2050, 0},
  {210, 0x1855, 0},
  {198, 0x1856, 0},
  {199, 0x9073, 0},
  {200, 0x9270, 0},
  {201, 0xa189, 0},
  {202, 0x9877, 0},
  {203, 0xc273, 0},
  {191, 0x8473, 0},
  {193, 0x3558, 0},
  {194, 0x1159, 0},
  {195, 0xb388, 0},
  {196, 0x2451, 0},
  {178, 0xa473, 0},
  {179, 0x660, 0},
  {180, 0x3559, 0},
  {181, 0xb172, 0},
  {182, 0x2853, 0},
  {169, 0x1459, 0},
  {170, 0xb671, 0},
  {171, 0x2258, 0},
  {172, 0x3554, 0},
  {173, 0xc170, 0},
  {174, 0x9273, 0},
  {175, 0x2459, 0},
  {161, 0xa390, 0},
  {162, 0x2854, 0},
  {163, 0x8774, 0},
  {164, 0xa077, 0},
  {165, 0x2550, 0},
  {152, 0xa173, 0},
  {153, 0xb572, 0},
  {154, 0xb071, 0},
  {155, 0xb372, 0},
  {156, 0x3759, 0},
  {157, 0x2755, 0},
  {158, 0x2154, 0},
  {143, 0x3862, 0},
  {144, 0x1759, 0},
  {145, 0xb373, 0},
  {146, 0xb189, 0},
  {147, 0x2151, 0},
  {148, 0xb370, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_43
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0},
  {287, 0xb982, 0},
  {286, 0xa969, 0},
  {285, 0xa583, 0},
  {284, 0x9875, 0},
  {283, 0xb781, 0},
  {282, 0xa283, 0},
  {281, 0xa582, 0},
  {280, 0xb381, 0},
  {279, 0x9780, 0},
  {278, 0xa968, 0},
  {277, 0xc176, 0},
  {276, 0x9482, 0},
  {275, 0x9771, 0},
  {274, 0xa880, 0},
  {273, 0x9279, 0},
  {272, 0x2861, 0},
  {271, 0xa980, 0},
  {270, 0xb380, 0},
  {269, 0xb477, 0},
  {268, 0x9681, 0},
  {267, 0xb182, 0},
  {266, 0xc369, 0},
  {265, 0xa470, 0},
  {264, 0x9575, 0},
  {263, 0x8470, 0},
  {261, 0x9483, 0},
  {260, 0xa478, 0},
  {259, 0xa580, 0},
  {258, 0x1860, 0},
  {257, 0xb668, 0},
  {256, 0x1660, 0},
  {255, 0xb168, 0},
  {254, 0x8982, 0},
  {253, 0xc277, 0},
  {252, 0xb580, 0},
  {251, 0x9876, 0},
  {250, 0xa483, 0},
  {249, 0x9583, 0},
  {248, 0xb383, 0},
  {247, 0xa982, 0},
  {246, 0xb881, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_63
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0},
  {287, 0xb982, 0},
  {286, 0xa969, 0},
  {285, 0xa583, 0},
  {284, 0x9875, 0},
  {283, 0xb781, 0},
  {282, 0xa283, 0},
  {281, 0xa582, 0},
  {280, 0xb381, 0},
  {279, 0x9780, 0},
  {278, 0xa968, 0},
  {277, 0xc176, 0},
  {276, 0x9482, 0},
  {275, 0x9771, 0},
  {274, 0xa880, 0},
  {273, 0x9279, 0},
  {272, 0x2861, 0},
  {271, 0xa980, 0},
  {270, 0xb380, 0},
  {269, 0xb477, 0},
  {268, 0x9681, 0},
  {267, 0xb182, 0},
  {266, 0xc369, 0},
  {265, 0xa470, 0},
  {264, 0x9575, 0},
  {263, 0x8470, 0},
  {261, 0x9483, 0},
  {260, 0xa478, 0},
  {259, 0xa580, 0},
  {258, 0x1860, 0},
  {257, 0xb668, 0},
  {256, 0x1660, 0},
  {255, 0xb168, 0},
  {254, 0x8982, 0},
  {253, 0xc277, 0},
  {252, 0xb580, 0},
  {251, 0x9876, 0},
  {250, 0xa483, 0},
  {249, 0x9583, 0},
  {248, 0xb383, 0},
  {247, 0xa982, 0},
  {246, 0xb881, 0},
  {245, 0x9269, 0},
  {244, 0x9579, 0},
  {242, 0x9077, 0},
  {241, 0x8371, 0},
  {240, 0x3761, 0},
  {239, 0xb876, 0},
  {238, 0xa083, 0},
  {237, 0x1562, 0},
  {236, 0xa677, 0},
  {235, 0x2362, 0},
  {234, 0x9481, 0},
  {233, 0x8881, 0},
  {232, 0xa372, 0},
  {231, 0x8779, 0},
  {230, 0x3861, 0},
  {229, 0xb068, 0},
  {228, 0x9283, 0},
  {227, 0x9377, 0},
  {226, 0xb268, 0},
  {225, 0xc068, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_83
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0},
  {287, 0xb982, 0},
  {286, 0xa969, 0},
  {285, 0xa583, 0},
  {284, 0x9875, 0},
  {283, 0xb781, 0},
  {282, 0xa283, 0},
  {281, 0xa582, 0},
  {280, 0xb381, 0},
  {279, 0x9780, 0},
  {278, 0xa968, 0},
  {277, 0xc176, 0},
  {276, 0x9482, 0},
  {275, 0x9771, 0},
  {274, 0xa880, 0},
  {273, 0x9279, 0},
  {272, 0x2861, 0},
  {271, 0xa980, 0},
  {270, 0xb380, 0},
  {269, 0xb477, 0},
  {268, 0x9681, 0},
  {267, 0xb182, 0},
  {266, 0xc369, 0},
  {265, 0xa470, 0},
  {264, 0x9575, 0},
  {263, 0x8470, 0},
  {261, 0x9483, 0},
  {260, 0xa478, 0},
  {259, 0xa580, 0},
  {258, 0x1860, 0},
  {257, 0xb668, 0},
  {256, 0x1660, 0},
  {255, 0xb168, 0},
  {254, 0x8982, 0},
  {253, 0xc277, 0},
  {252, 0xb580, 0},
  {251, 0x9876, 0},
  {250, 0xa483, 0},
  {249, 0x9583, 0},
  {248, 0xb383, 0},
  {247, 0xa982, 0},
  {246, 0xb881, 0},
  {245, 0x9269, 0},
  {244, 0x9579, 0},
  {242, 0x9077, 0},
  {241, 0x8371, 0},
  {240, 0x3761, 0},
  {239, 0xb876, 0},
  {238, 0xa083, 0},
  {237, 0x1562, 0},
  {236, 0xa677, 0},
  {235, 0x2362, 0},
  {234, 0x9481, 0},
  {233, 0x8881, 0},
  {232, 0xa372, 0},
  {231, 0x8779, 0},
  {230, 0x3861, 0},
  {229, 0xb068, 0},
  {228, 0x9283, 0},
  {227, 0x9377, 0},
  {226, 0xb268, 0},
  {225, 0xc068, 0},
  {224, 0xb180, 0},
  {223, 0xb768, 0},
  {222, 0x3860, 0},
  {221, 0x9981, 0},
  {220, 0x2160, 0},
  {219, 0xb481, 0},
  {218, 0xb582, 0},
  {217, 0x9871, 0},
  {216, 0xb369, 0},
  {215, 0x9175, 0},
  {214, 0x9567, 0},
  {213, 0x8472, 0},
  {212, 0xb280, 0},
  {211, 0x8875, 0},
  {210, 0xc081, 0},
  {209, 0xb379, 0},
  {208, 0x9076, 0},
  {207, 0xa379, 0},
  {205, 0x8967, 0},
  {204, 0xb877, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_2
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_3
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {289, 0xc370, 0}, // root node
  {288, 0x1162, 0},
  {287, 0xb982, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_L_43
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {95, 0xa770, 0}, // root node
  {96, 0xb468, 0},
  {97, 0xb179, 0},
  {98, 0xb069, 0},
  {99, 0xb277, 0},
  {101, 0x9181, 0},
  {103, 0x9881, 0},
  {104, 0xa775, 0},
  {105, 0xb576, 0},
  {106, 0x9382, 0},
  {107, 0xa072, 0},
  {108, 0x8477, 0},
  {109, 0x1062, 0},
  {110, 0xa071, 0},
  {111, 0x9172, 0},
  {112, 0xa578, 0},
  {113, 0xa671, 0},
  {114, 0xa879, 0},
  {115, 0x8471, 0},
  {116, 0x8969, 0},
  {117, 0x2262, 0},
  {118, 0x9376, 0},
  {119, 0xa468, 0},
  {120, 0xa469, 0},
  {121, 0x8572, 0},
  {122, 0xa180, 0},
  {123, 0xc276, 0},
  {124, 0x9772, 0},
  {125, 0x9672, 0},
  {126, 0xb269, 0},
  {127, 0x8671, 0},
  {128, 0x9980, 0},
  {129, 0x9783, 0},
  {130, 0xa272, 0},
  {131, 0x9369, 0},
  {132, 0xa369, 0},
  {133, 0x2360, 0},
  {134, 0xa069, 0},
  {135, 0x1761, 0},
  {136, 0x9669, 0},
  {137, 0x9371, 0},
  {138, 0xb169, 0},
  {139, 0xa778, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_L_63
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {95, 0xa770, 0}, // root node
  {96, 0xb468, 0},
  {97, 0xb179, 0},
  {98, 0xb069, 0},
  {99, 0xb277, 0},
  {101, 0x9181, 0},
  {103, 0x9881, 0},
  {104, 0xa775, 0},
  {105, 0xb576, 0},
  {106, 0x9382, 0},
  {107, 0xa072, 0},
  {108, 0x8477, 0},
  {109, 0x1062, 0},
  {110, 0xa071, 0},
  {111, 0x9172, 0},
  {112, 0xa578, 0},
  {113, 0xa671, 0},
  {114, 0xa879, 0},
  {115, 0x8471, 0},
  {116, 0x8969, 0},
  {117, 0x2262, 0},
  {118, 0x9376, 0},
  {119, 0xa468, 0},
  {120, 0xa469, 0},
  {121, 0x8572, 0},
  {122, 0xa180, 0},
  {123, 0xc276, 0},
  {124, 0x9772, 0},
  {125, 0x9672, 0},
  {126, 0xb269, 0},
  {127, 0x8671, 0},
  {128, 0x9980, 0},
  {129, 0x9783, 0},
  {130, 0xa272, 0},
  {131, 0x9369, 0},
  {132, 0xa369, 0},
  {133, 0x2360, 0},
  {134, 0xa069, 0},
  {135, 0x1761, 0},
  {136, 0x9669, 0},
  {137, 0x9371, 0},
  {138, 0xb169, 0},
  {139, 0xa778, 0},
  {140, 0xb483, 0},
  {141, 0x8579, 0},
  {142, 0xa783, 0},
  {143, 0x9779, 0},
  {144, 0xc069, 0},
  {145, 0x9680, 0},
  {146, 0xa668, 0},
  {147, 0x9380, 0},
  {148, 0x9976, 0},
  {149, 0x9683, 0},
  {150, 0xb676, 0},
  {151, 0x9576, 0},
  {152, 0xb769, 0},
  {153, 0xb081, 0},
  {154, 0xa279, 0},
  {156, 0x1861, 0},
  {157, 0xa380, 0},
  {159, 0xa081, 0},
  {160, 0x9880, 0},
  {161, 0x9582, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_L_79
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {95, 0xa770, 0}, // root node
  {96, 0xb468, 0},
  {97, 0xb179, 0},
  {98, 0xb069, 0},
  {99, 0xb277, 0},
  {101, 0x9181, 0},
  {103, 0x9881, 0},
  {104, 0xa775, 0},
  {105, 0xb576, 0},
  {106, 0x9382, 0},
  {107, 0xa072, 0},
  {108, 0x8477, 0},
  {109, 0x1062, 0},
  {110, 0xa071, 0},
  {111, 0x9172, 0},
  {112, 0xa578, 0},
  {113, 0xa671, 0},
  {114, 0xa879, 0},
  {115, 0x8471, 0},
  {116, 0x8969, 0},
  {117, 0x2262, 0},
  {118, 0x9376, 0},
  {119, 0xa468, 0},
  {120, 0xa469, 0},
  {121, 0x8572, 0},
  {122, 0xa180, 0},
  {123, 0xc276, 0},
  {124, 0x9772, 0},
  {125, 0x9672, 0},
  {126, 0xb269, 0},
  {127, 0x8671, 0},
  {128, 0x9980, 0},
  {129, 0x9783, 0},
  {130, 0xa272, 0},
  {131, 0x9369, 0},
  {132, 0xa369, 0},
  {133, 0x2360, 0},
  {134, 0xa069, 0},
  {135, 0x1761, 0},
  {136, 0x9669, 0},
  {137, 0x9371, 0},
  {138, 0xb169, 0},
  {139, 0xa778, 0},
  {140, 0xb483, 0},
  {141, 0x8579, 0},
  {142, 0xa783, 0},
  {143, 0x9779, 0},
  {144, 0xc069, 0},
  {145, 0x9680, 0},
  {146, 0xa668, 0},
  {147, 0x9380, 0},
  {148, 0x9976, 0},
  {149, 0x9683, 0},
  {150, 0xb676, 0},
  {151, 0x9576, 0},
  {152, 0xb769, 0},
  {153, 0xb081, 0},
  {154, 0xa279, 0},
  {156, 0x1861, 0},
  {157, 0xa380, 0},
  {159, 0xa081, 0},
  {160, 0x9880, 0},
  {161, 0x9582, 0},
  {162, 0x8775, 0},
  {163, 0x9276, 0},
  {164, 0xb281, 0},
  {166, 0x9671, 0},
  {167, 0x1862, 0},
  {168, 0x8475, 0},
  {169, 0xa276, 0},
  {170, 0xc076, 0},
  {171, 0x9379, 0},
  {172, 0x9678, 0},
  {173, 0x9381, 0},
  {174, 0xa179, 0},
  {175, 0xa870, 0},
  {176, 0x9572, 0},
  {177, 0x3061, 0},
  {178, 0xa281, 0}
};
#elif IOTLAB_SITE == IOTLAB_GRENOBLE_R_83_2
uint16_t iotlab_nodes[NODE_NUM][3] = {
  // {host name, uid, rx count}
  {242, 0x9077, 0}, // root node
  {289, 0xc370, 0},
  {288, 0x1162, 0},
  {287, 0xb982, 0},
  {286, 0xa969, 0},
  {285, 0xa583, 0},
  {284, 0x9875, 0},
  {283, 0xb781, 0},
  {282, 0xa283, 0},
  {281, 0xa582, 0},
  {280, 0xb381, 0},
  {279, 0x9780, 0},
  {278, 0xa968, 0},
  {277, 0xc176, 0},
  {276, 0x9482, 0},
  {275, 0x9771, 0},
  {274, 0xa880, 0},
  {273, 0x9279, 0},
  {272, 0x2861, 0},
  {271, 0xa980, 0},
  {270, 0xb380, 0},
  {269, 0xb477, 0},
  {268, 0x9681, 0},
  {267, 0xb182, 0},
  {266, 0xc369, 0},
  {265, 0xa470, 0},
  {264, 0x9575, 0},
  {263, 0x8470, 0},
  {261, 0x9483, 0},
  {260, 0xa478, 0},
  {259, 0xa580, 0},
  {258, 0x1860, 0},
  {257, 0xb668, 0},
  {256, 0x1660, 0},
  {255, 0xb168, 0},
  {254, 0x8982, 0},
  {253, 0xc277, 0},
  {252, 0xb580, 0},
  {251, 0x9876, 0},
  {250, 0xa483, 0},
  {249, 0x9583, 0},
  {248, 0xb383, 0},
  {247, 0xa982, 0},
  {246, 0xb881, 0},
  {245, 0x9269, 0},
  {244, 0x9579, 0},
  {241, 0x8371, 0},
  {240, 0x3761, 0},
  {239, 0xb876, 0},
  {238, 0xa083, 0},
  {237, 0x1562, 0},
  {236, 0xa677, 0},
  {235, 0x2362, 0},
  {234, 0x9481, 0},
  {233, 0x8881, 0},
  {232, 0xa372, 0},
  {231, 0x8779, 0},
  {230, 0x3861, 0},
  {229, 0xb068, 0},
  {228, 0x9283, 0},
  {227, 0x9377, 0},
  {226, 0xb268, 0},
  {225, 0xc068, 0},
  {224, 0xb180, 0},
  {223, 0xb768, 0},
  {222, 0x3860, 0},
  {221, 0x9981, 0},
  {220, 0x2160, 0},
  {219, 0xb481, 0},
  {218, 0xb582, 0},
  {217, 0x9871, 0},
  {216, 0xb369, 0},
  {215, 0x9175, 0},
  {214, 0x9567, 0},
  {213, 0x8472, 0},
  {212, 0xb280, 0},
  {211, 0x8875, 0},
  {210, 0xc081, 0},
  {209, 0xb379, 0},
  {208, 0x9076, 0},
  {207, 0xa379, 0},
  {205, 0x8967, 0},
  {204, 0xb877, 0}
};
#endif
*/