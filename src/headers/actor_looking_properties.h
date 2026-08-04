#pragma once
/* actor_looking_properties — actor_definition.looking (152 bytes), per an authoritative
 * structure dump (C:\halo\headers_ref\actor_looking_properties.h). actor_idle_looking is
 * fully named per C:\halo\headers_ref\actor_idle_looking.h — its idle_look_time bounds are
 * read directly by action_obey_command_begin.c's atom 0x17 ("look at random point in
 * range") for the default look-duration roll. */

#include "real_euler_angles2d.h"
#include "actor_idle_looking.h"

typedef struct actor_looking_properties
{
    real_euler_angles2d  maximum_aiming_deviation;            /* 0x00 */
    real_euler_angles2d  maximum_looking_deviation;            /* 0x08 */
    float                noncombat_looking_delta_angles[2];     /* 0x10 */
    float                combat_looking_delta_angles[2];         /* 0x18 */
    real_euler_angles2d  idle_aiming_deviation;                  /* 0x20 */
    real_euler_angles2d  idle_looking_deviation;                  /* 0x28 */
    float                event_look_modifier_lower_bound;          /* 0x30 */
    float                event_look_modifier_upper_bound;           /* 0x34 */
    actor_idle_looking   idle_look_noncombat;                        /* 0x38 */
    actor_idle_looking   idle_look_guard;                              /* 0x50 */
    actor_idle_looking   idle_look_search_combat;                       /* 0x68 */
    float                unused[2];                                     /* 0x80 */
    real_euler_angles2d  runtime_maximum_aiming_deviation_cosine;        /* 0x88 */
    real_euler_angles2d  runtime_maximum_looking_deviation_cosine;        /* 0x90 */
} actor_looking_properties; /* 0x98 = 152 bytes */
