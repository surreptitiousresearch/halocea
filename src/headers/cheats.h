#pragma once
/* cheats — runtime cheat-toggle flags (one byte each). Global instance is `cheat`. */
// DB-verified via types_members cheat_globals (reconstructed source spelled it `cheats`).

/* Shared struct guard: cheat_globals.h defines the identical layout; guard lets both coexist. */
#ifndef BLAM_CHEAT_GLOBALS_STRUCT
#define BLAM_CHEAT_GLOBALS_STRUCT
typedef struct cheat_globals
{
    unsigned __int8 deathless_player;          /* 0x00 */
    unsigned __int8 jetpack;                   /* 0x01 */
    unsigned __int8 infinite_ammo;             /* 0x02 */
    unsigned __int8 bump_possession;           /* 0x03 */
    unsigned __int8 super_jump;                /* 0x04 */
    unsigned __int8 reflexive_damage_effects;  /* 0x05 */
    unsigned __int8 medusa;                    /* 0x06 */
    unsigned __int8 omnipotent;                /* 0x07 */
    unsigned __int8 controller_enabled;        /* 0x08 */
    unsigned __int8 bottomless_clip;           /* 0x09 */
} cheat_globals;
#endif

typedef cheat_globals cheats;  /* Blam source spelling; DB type is cheat_globals */

extern cheats cheat;
