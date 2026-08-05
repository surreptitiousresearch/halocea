/* collision_model_test_pill_result — output of collision_model_test_pill: which node/region/permutation of the
 * model the swept pill struck, plus the underlying BSP-test result (whose .fraction doubles as the running best-t
 * the model test passes back into each per-node collision_bsp_test_pill call). */
#pragma once

#include <stdint.h>
#include "collision_bsp_test_pill_result.h"

typedef struct collision_model_test_pill_result
{
    int16_t                        node_index;        /* 0x00 */
    int16_t                        region_index;      /* 0x02 */
    int16_t                        bsp_index; /* 0x04 */
    int16_t                        _pad06;            /* 0x06 */
    collision_bsp_test_pill_result bsp_result;        /* 0x08 — .fraction = best contact fraction so far */
} collision_model_test_pill_result;
