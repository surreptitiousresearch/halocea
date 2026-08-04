#pragma once
/* item_accelerate_network_data — payload of the _message_item_accelerate message-delta
 * (headers_ref item_accelerate_network_data, sizeof=0x14). */

#include "real_vector3d.h"

typedef struct item_accelerate_network_data
{
    int           object_index;   /* 0x00 */
    float         magnitude;      /* 0x04 */
    real_vector3d direction;      /* 0x08 */
} item_accelerate_network_data;   /* 0x14 */
