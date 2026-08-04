#pragma once
/* cheat_globals — runtime cheat toggle flags (cheats.c). Layout from the database (10 bytes). */

/* Shared struct guard: cheats.h defines the identical layout; guard lets both coexist. */
#ifndef BLAM_CHEAT_GLOBALS_STRUCT
#define BLAM_CHEAT_GLOBALS_STRUCT
typedef struct cheat_globals
{
    unsigned __int8 deathless_player;          /* 0x0 */
    unsigned __int8 jetpack;                    /* 0x1 */
    unsigned __int8 infinite_ammo;              /* 0x2 */
    unsigned __int8 bump_possession;            /* 0x3 */
    unsigned __int8 super_jump;                 /* 0x4 */
    unsigned __int8 reflexive_damage_effects;   /* 0x5 */
    unsigned __int8 medusa;                     /* 0x6 */
    unsigned __int8 omnipotent;                 /* 0x7 */
    unsigned __int8 controller_enabled;         /* 0x8 */
    unsigned __int8 bottomless_clip;            /* 0x9 */
} cheat_globals;                                /* 10 bytes */
#endif

extern cheat_globals cheat;
