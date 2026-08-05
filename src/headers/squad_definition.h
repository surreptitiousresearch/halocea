#pragma once
/* squad_definition — element of scenario.ai_squad_definitions (an AI squad placement entry). DB layout,
 * 232 bytes. */

#include <stdint.h>
#include "tag_block.h"

typedef struct squad_definition
{
    char             name[32];                    /* 0x00 */
    int16_t          actor_palette_index;          /* 0x20 */
    int16_t          platoon_index;                /* 0x22 */
    int16_t          initial_state;                /* 0x24 */
    int16_t          default_state;                /* 0x26 */
    unsigned int     flags;                        /* 0x28 */
    int16_t          unique_leader_type;            /* 0x2C */
    uint16_t pad;                          /* 0x2E */
    unsigned int     unused1[7];                   /* 0x30 */
    uint16_t pad5;                         /* 0x4C */
    int16_t          maneuver_squad_index;          /* 0x4E */
    float            squad_delay_timer;             /* 0x50 */
    unsigned int     firing_position_groups[8];     /* 0x54 */
    unsigned int     pad2[2];                       /* 0x74 */
    int16_t          min_count;                     /* 0x7C */
    int16_t          max_count;                     /* 0x7E */
    int16_t          major_upgrade;                 /* 0x80 */
    uint16_t pad3;                          /* 0x82 */
    int16_t          respawn_min_actors;            /* 0x84 */
    int16_t          respawn_max_actors;            /* 0x86 */
    int16_t          respawn_total_count;           /* 0x88 */
    uint16_t pad4;                          /* 0x8A */
    float            respawn_time_lower_bound;       /* 0x8C */
    float            respawn_time_upper_bound;       /* 0x90 */
    unsigned int     unused3[12];                   /* 0x94 */
    tag_block        move_positions;                /* 0xC4 */
    tag_block        starting_locations;             /* 0xD0 */
    tag_block        unused_block;                  /* 0xDC */
} squad_definition; /* 232 bytes */
