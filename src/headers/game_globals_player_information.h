#pragma once
/* game_globals_player_information — one entry of game_globals.player_information (the 'matg'
 * player-information block, 244 bytes): the player unit tag, movement speeds, grenade origin,
 * stun penalties, and first-person idle timing. Layout DB-verified (2026-07-14). */

#include "tag_reference.h"
#include "real_vector3d.h"

typedef struct game_globals_player_information
{
    tag_reference player_unit;                        /* 0x00 */
    int           unused1[7];                         /* 0x10 */
    float         walking_speed;                      /* 0x2C (44) */
    float         double_speed_multiplier;            /* 0x30 (48) */
    float         run_forward_speed;                  /* 0x34 (52) */
    float         run_backward_speed;                 /* 0x38 (56) */
    float         run_sideways_speed;                 /* 0x3C (60) */
    float         run_acceleration;                   /* 0x40 (64) */
    float         sneak_forward_speed;                /* 0x44 (68) */
    float         sneak_backward_speed;               /* 0x48 (72) */
    float         sneak_sideways_speed;               /* 0x4C (76) */
    float         sneak_acceleration;                 /* 0x50 (80) */
    float         airborne_acceleration;              /* 0x54 (84) */
    float         multiplayer_only_speed_muliplier;   /* 0x58 (88) — DB spelling */
    int           movement_unused[3];                 /* 0x5C (92) */
    real_vector3d grenade_origin;                     /* 0x68 (104) */
    float         grenade_unused[3];                  /* 0x74 (116) */
    float         stun_movement_penalty;              /* 0x80 (128) */
    float         stun_turning_penalty;               /* 0x84 (132) */
    float         stun_jumping_penalty;               /* 0x88 (136) */
    float         minimum_stun_time;                  /* 0x8C (140) */
    float         maximum_stun_time;                  /* 0x90 (144) */
    float         stun_unused[2];                     /* 0x94 (148) */
    float         first_person_idle_time_lower_bound; /* 0x9C (156) */
    float         first_person_idle_time_upper_bound; /* 0xA0 (160) */
    float         first_person_idle_skip_fraction;    /* 0xA4 (164) */
    int           unused_first_person_unused[4];      /* 0xA8 (168) */
    tag_reference coop_respawn_effect;                /* 0xB8 (184) */
    int           unused2[11];                        /* 0xC8 (200) */
} game_globals_player_information;                    /* 244 bytes */
