#pragma once
#include <stdint.h>
/* cheat_globals — runtime cheat toggle flags (cheats.c). Layout from the database (10 bytes). */

/* Shared struct guard: cheats.h defines the identical layout; guard lets both coexist. */
#ifndef BLAM_CHEAT_GLOBALS_STRUCT
#define BLAM_CHEAT_GLOBALS_STRUCT
typedef struct cheat_globals
{
    uint8_t deathless_player;          /* 0x0 */
    uint8_t jetpack;                    /* 0x1 */
    uint8_t infinite_ammo;              /* 0x2 */
    uint8_t bump_possession;            /* 0x3 */
    uint8_t super_jump;                 /* 0x4 */
    uint8_t reflexive_damage_effects;   /* 0x5 */
    uint8_t medusa;                     /* 0x6 */
    uint8_t omnipotent;                 /* 0x7 */
    uint8_t controller_enabled;         /* 0x8 */
    uint8_t bottomless_clip;            /* 0x9 */
} cheat_globals;                                /* 10 bytes */
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern cheat_globals cheat;

#ifdef __cplusplus
}
#endif
