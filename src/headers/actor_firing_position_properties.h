#pragma once
/* actor_firing_position_properties — actor_definition.firing_position (64 bytes), per an
 * authoritative structure dump (C:\halo\headers_ref\actor_firing_position_properties.h). Sits at
 * absolute byte 952. guard_position_time_lower_bound/upper_bound resolve the doc's "guard-delay
 * seconds (min/max at tag +952/+956)" citation (action_guard_perform.c's re-check timer) exactly. */

typedef struct actor_firing_position_properties
{
    float         guard_position_time_lower_bound;    /* 0x00 (abs 952) — doc's "tag +952" guard-delay lower bound */
    float         guard_position_time_upper_bound;      /* 0x04 (abs 956) — doc's "tag +956" guard-delay upper bound */
    float         combat_position_time_lower_bound;       /* 0x08 (abs 960) */
    float         combat_position_time_upper_bound;         /* 0x0C (abs 964) */
    float         old_avoidance_radius;                        /* 0x10 (abs 968) */
    float         friend_avoidance_radius;                       /* 0x14 (abs 972) */
    unsigned int  unused_firing_position[10];                      /* 0x18 */
} actor_firing_position_properties; /* 0x40 = 64 bytes */
