#pragma once
/* actor_movement_switching_properties — an actor_variant_definition's crouch/run switching timing (24 bytes,
 * DB-verified). */

typedef struct actor_movement_switching_properties
{
    __int16 movement_type;              /* 0x00 */
    unsigned __int16 pad;               /* 0x02 */
    float   initial_crouch_chance;      /* 0x04 */
    float   crouch_time_lower_bound;    /* 0x08 */
    float   crouch_time_upper_bound;    /* 0x0C */
    float   run_time_lower_bound;       /* 0x10 */
    float   run_time_upper_bound;       /* 0x14 */
} actor_movement_switching_properties;  /* 0x18 = 24 bytes */
