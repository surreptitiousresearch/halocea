#pragma once
/* actor_look_orders — member of actor_orders at relative 0x00, 24 bytes (per the authoritative
 * actor_datum structure dump). Per structure dump from C:/halo/headers_ref/. */

#include <stdint.h>
#include "direction_specification.h"

typedef struct actor_look_orders
{
    int16_t primary_priority;           /* 0x00 */
    unsigned char _pad02[2];            /* 0x02 */
    direction_specification primary_direction; /* 0x04 */
    int16_t idle_look_type;             /* 0x14 */
    unsigned char _pad16[2];            /* 0x16 */
} actor_look_orders; /* 24 bytes */
