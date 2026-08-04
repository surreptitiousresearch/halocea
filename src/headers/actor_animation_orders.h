#pragma once
/* actor_animation_orders — member of actor_output_data at relative 0x1C, 12 bytes (per the authoritative
 * actor_datum structure dump). Per structure dump from C:/halo/headers_ref/. */

typedef struct actor_animation_orders
{
    __int16 impulse;                    /* 0x00 */
    unsigned char _pad02[2];            /* 0x02 */
    real_vector2d alignment_vector;     /* 0x04 */
} actor_animation_orders; /* 12 bytes */
