#pragma once
/* weapon_trigger — runtime state of one weapon trigger, embedded as weapon_datum.weapon.triggers[2].
 * Layout DB-verified against type 'weapon_trigger' (40 bytes, 2026-07-14). */

typedef struct weapon_trigger
{
    char             idle_ticks;                            /* 0x00 */
    char             state;                                 /* 0x01 */
    __int16          state_timer;                           /* 0x02 */
    unsigned int     flags;                                 /* 0x04 */
    unsigned __int16 firing_effects_used_flags;             /* 0x08 */
    __int16          firing_effect_index;                   /* 0x0A */
    __int16          firing_effect_shots_remaining;         /* 0x0C */
    __int16          sequential_non_tracer_rounds;          /* 0x0E */
    float            rate_of_fire;                          /* 0x10 */
    float            ejection_port_position;                /* 0x14 */
    float            illumination;                          /* 0x18 */
    float            __noop;                                /* 0x1C */
    int              charging_effect_index;                 /* 0x20 */
    char             delay_ticks_before_empty_clip_auto_reload; /* 0x24 */
    char             pad[3];                                /* 0x25 */
} weapon_trigger;                                           /* 40 bytes */
