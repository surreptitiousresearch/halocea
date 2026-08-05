#pragma once
#include <stdint.h>
/* squad_datum — runtime per-squad state (32 bytes, full DB layout). */

typedef struct squad_datum
{
    unsigned int    required_locations[1];        /* 0x00 */
    unsigned int    unused_locations[1];          /* 0x04 */
    float           major_upgrade_error;          /* 0x08 */
    int16_t         respawn_actors_left;          /* 0x0C */
    int16_t         respawn_delay_ticks;          /* 0x0E */
    uint8_t automatic_migration_target;   /* 0x10 */
    uint8_t delay_timer_started;          /* 0x11 */
    int16_t         delay_timer;                  /* 0x12 */
    uint8_t disable_dormant;              /* 0x14 */
    uint8_t _pad15;                       /* 0x15 */
    int16_t         original_count;               /* 0x16 */
    int16_t         current_count;                /* 0x18 */
    int16_t         current_swarm_count;          /* 0x1A */
    float           current_strength_fraction;    /* 0x1C */
} squad_datum;                                    /* 32 bytes */
