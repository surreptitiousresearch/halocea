#pragma once
#include <stdint.h>
/* cheats — runtime cheat-toggle flags (one byte each). Global instance is `cheat`. */
// DB-verified via types_members cheat_globals (reconstructed source spelled it `cheats`).

/* Shared struct guard: cheat_globals.h defines the identical layout; guard lets both coexist. */
#ifndef BLAM_CHEAT_GLOBALS_STRUCT
#define BLAM_CHEAT_GLOBALS_STRUCT
typedef struct cheat_globals
{
    uint8_t deathless_player;          /* 0x00 */
    uint8_t jetpack;                   /* 0x01 */
    uint8_t infinite_ammo;             /* 0x02 */
    uint8_t bump_possession;           /* 0x03 */
    uint8_t super_jump;                /* 0x04 */
    uint8_t reflexive_damage_effects;  /* 0x05 */
    uint8_t medusa;                    /* 0x06 */
    uint8_t omnipotent;                /* 0x07 */
    uint8_t controller_enabled;        /* 0x08 */
    uint8_t bottomless_clip;           /* 0x09 */
} cheat_globals;
#endif

typedef cheat_globals cheats;  /* Blam source spelling; DB type is cheat_globals */

extern cheats cheat;
