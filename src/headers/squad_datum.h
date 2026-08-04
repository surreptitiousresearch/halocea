#pragma once
/* squad_datum — runtime per-squad state (32 bytes, full DB layout). */

typedef struct squad_datum
{
    unsigned int    required_locations[1];        /* 0x00 */
    unsigned int    unused_locations[1];          /* 0x04 */
    float           major_upgrade_error;          /* 0x08 */
    __int16         respawn_actors_left;          /* 0x0C */
    __int16         respawn_delay_ticks;          /* 0x0E */
    unsigned __int8 automatic_migration_target;   /* 0x10 */
    unsigned __int8 delay_timer_started;          /* 0x11 */
    __int16         delay_timer;                  /* 0x12 */
    unsigned __int8 disable_dormant;              /* 0x14 */
    unsigned __int8 _pad15;                       /* 0x15 */
    __int16         original_count;               /* 0x16 */
    __int16         current_count;                /* 0x18 */
    __int16         current_swarm_count;          /* 0x1A */
    float           current_strength_fraction;    /* 0x1C */
} squad_datum;                                    /* 32 bytes */
