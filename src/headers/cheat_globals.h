#pragma once
#include <stdint.h>
/* cheat_globals — runtime cheat toggle flags (cheats.c). DB-verified: types_members cheat_globals,
 * ten uint8_t flags at 0x0..0x9, size 10. Sole definition; cheats.h includes this and adds the
 * `cheats` source-spelling typedef. */

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

#ifdef __cplusplus
extern "C" {
#endif

extern cheat_globals cheat;

#ifdef __cplusplus
}
#endif
