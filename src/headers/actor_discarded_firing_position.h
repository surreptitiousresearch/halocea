#pragma once
#include <stdint.h>
/* actor_discarded_firing_position — element of actor_firing_position_data.discarded_firing_positions[4]
 * at relative 0x10, 4 bytes each. Per authoritative structure dump from C:/halo/headers_ref/. */

typedef struct actor_discarded_firing_position
{
    unsigned char temporary;    /* 0x00 */
    unsigned char pad;          /* 0x01 */
    int16_t index;              /* 0x02 */
} actor_discarded_firing_position; /* 4 bytes */
