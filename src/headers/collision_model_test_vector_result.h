/* collision_model_test_vector_result — nearest hit of a ray against an object's collision model: the
 * node/region/bsp it struck plus the underlying BSP test result (1056 bytes). */
#pragma once

#include <stdint.h>
#include "collision_bsp_test_vector_result.h"

typedef struct collision_model_test_vector_result
{
    int16_t                          node_index;   /* 0x00 */
    int16_t                          region_index; /* 0x02 */
    int16_t                          bsp_index;    /* 0x04 */
    char                             pad_06[2];    /* 0x06 */
    collision_bsp_test_vector_result bsp_result;   /* 0x08 */
} collision_model_test_vector_result; /* 1056 bytes */
