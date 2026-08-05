#pragma once
/* universal_variant — engine-agnostic multiplayer variant settings block (72 bytes).
 * Embedded in game_variant at offset 0x34. DB-verified against PDB type universal_variant. */

#include <stdint.h>
#include "vehicle_options_s.h"

typedef struct universal_variant
{
    uint8_t   teams;                 /* 0x00 */
    uint8_t   pad0;                  /* 0x01 */
    uint8_t   pad1;                  /* 0x02 */
    uint8_t   pad2;                  /* 0x03 */
    int               flags;                 /* 0x04 */
    int               goal_radar;            /* 0x08 */
    uint8_t   odd_man_out;           /* 0x0C */
    uint8_t   pad4;                  /* 0x0D */
    uint8_t   pad5;                  /* 0x0E */
    uint8_t   pad6;                  /* 0x0F */
    int               respawn_time_growth;   /* 0x10 */
    int               respawn_time;          /* 0x14 */
    int               suicide_penalty;       /* 0x18 */
    int               lives;                 /* 0x1C */
    float             health;                /* 0x20 */
    int               score_to_win;          /* 0x24 */
    int               weapon_set;            /* 0x28 */
    vehicle_options_s vehicle_set;           /* 0x2C */
    vehicle_options_s vehicle_set2;          /* 0x30 */
    int               vehicles_respawn;      /* 0x34 — respawn delay in ticks (0 = disabled) */
    uint8_t   friendly_fire;         /* 0x38 */
    unsigned char     _pad7[3];              /* 0x39 db-verified padding */
    int               friendly_fire_penalty; /* 0x3C */
    uint8_t   team_autobalance;      /* 0x40 */
    unsigned char     _pad8[3];              /* 0x41 db-verified padding */
    int               time_limit;            /* 0x44 */
} universal_variant;                         /* 72 bytes */
