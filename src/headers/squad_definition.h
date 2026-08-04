#pragma once
/* squad_definition — element of scenario.ai_squad_definitions (an AI squad placement entry). DB layout,
 * 232 bytes. */

#include "tag_block.h"

typedef struct squad_definition
{
    char             name[32];                    /* 0x00 */
    __int16          actor_palette_index;          /* 0x20 */
    __int16          platoon_index;                /* 0x22 */
    __int16          initial_state;                /* 0x24 */
    __int16          default_state;                /* 0x26 */
    unsigned int     flags;                        /* 0x28 */
    __int16          unique_leader_type;            /* 0x2C */
    unsigned __int16 pad;                          /* 0x2E */
    unsigned int     unused1[7];                   /* 0x30 */
    unsigned __int16 pad5;                         /* 0x4C */
    __int16          maneuver_squad_index;          /* 0x4E */
    float            squad_delay_timer;             /* 0x50 */
    unsigned int     firing_position_groups[8];     /* 0x54 */
    unsigned int     pad2[2];                       /* 0x74 */
    __int16          min_count;                     /* 0x7C */
    __int16          max_count;                     /* 0x7E */
    __int16          major_upgrade;                 /* 0x80 */
    unsigned __int16 pad3;                          /* 0x82 */
    __int16          respawn_min_actors;            /* 0x84 */
    __int16          respawn_max_actors;            /* 0x86 */
    __int16          respawn_total_count;           /* 0x88 */
    unsigned __int16 pad4;                          /* 0x8A */
    float            respawn_time_lower_bound;       /* 0x8C */
    float            respawn_time_upper_bound;       /* 0x90 */
    unsigned int     unused3[12];                   /* 0x94 */
    tag_block        move_positions;                /* 0xC4 */
    tag_block        starting_locations;             /* 0xD0 */
    tag_block        unused_block;                  /* 0xDC */
} squad_definition; /* 232 bytes */
