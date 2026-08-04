#pragma once
/* universal_variant — engine-agnostic multiplayer variant settings block (72 bytes).
 * Embedded in game_variant at offset 0x34. DB-verified against PDB type universal_variant. */

#include "vehicle_options_s.h"

typedef struct universal_variant
{
    unsigned __int8   teams;                 /* 0x00 */
    unsigned __int8   pad0;                  /* 0x01 */
    unsigned __int8   pad1;                  /* 0x02 */
    unsigned __int8   pad2;                  /* 0x03 */
    int               flags;                 /* 0x04 */
    int               goal_radar;            /* 0x08 */
    unsigned __int8   odd_man_out;           /* 0x0C */
    unsigned __int8   pad4;                  /* 0x0D */
    unsigned __int8   pad5;                  /* 0x0E */
    unsigned __int8   pad6;                  /* 0x0F */
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
    unsigned __int8   friendly_fire;         /* 0x38 */
    unsigned char     _pad7[3];              /* 0x39 db-verified padding */
    int               friendly_fire_penalty; /* 0x3C */
    unsigned __int8   team_autobalance;      /* 0x40 */
    unsigned char     _pad8[3];              /* 0x41 db-verified padding */
    int               time_limit;            /* 0x44 */
} universal_variant;                         /* 72 bytes */
