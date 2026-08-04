#pragma once
/* game_globals — top-level Blam game state (game.c) */

#include "tag_block.h"
#include "game_options.h"       /* struct_game_globals.options */
#include "game_variant.h"       /* bundled sibling — transitively re-exported */
#include "achievements_info.h"  /* bundled sibling — transitively re-exported */

/* game_globals — the 'globals' tag definition (globals.tag), 0x1AC bytes.
 * NOTE: distinct type from the runtime struct_game_globals below; they share the
 * base name only. The 'game_globals' variable (ordinary namespace) points at the
 * runtime struct_game_globals, not at this tag struct (tag namespace). Both are
 * real, distinct DB types (game_globals=428B tag, struct_game_globals=276B runtime). */
struct game_globals
{
    /* layout bound to DB type: types_members game_globals */
    unsigned int flags;                     /* 0x000 */
    int          unused0[61];               /* 0x004 */
    tag_block    sounds;                    /* 0x0F8 (248) */
    tag_block    camera;                    /* 0x104 (260) */
    tag_block    player_control;            /* 0x110 (272) — element word index 55 (byte 0x6E) = auto-level threshold ticks */
    tag_block    difficulty_information;    /* 0x11C — element float[4]; address[4*value_type + difficulty] */
    tag_block    grenades;                  /* 0x128 — element dword 29 (+0x74) = grenade object definition index */
    tag_block    rasterizer_data;           /* 0x134 — rasterizer config block (game_globals_rasterizer_data) */
    tag_block    interface_tag_references;  /* 0x140 — element 16 bytes; +12 (dword 3) = interface tag index */
    tag_block    weapon_list;               /* 0x14C — element stride 16 dwords; +3 = weapon definition tag index */
    tag_block    cheat_powerups;            /* 0x158 */
    tag_block    multiplayer_information;   /* 0x164 — dword 9 (+0x24) = vehicle remapper info array base */
    tag_block    player_information;        /* 0x170 — element[0] float[33] = look-magnetism scale */
    tag_block    first_person_interface;    /* 0x17C — word[21]=night-vision-on effect, word[25]=night-vision-off */
    tag_block    falling_damage;            /* 0x188 — element int[7] (+28) = falling/distance damage effect index */
    tag_block    materials;                 /* 0x194 — 884-byte (0x374) records; breakable-surface effect
                                             *  sub-block at +0x2D4 (sound def@+44, emitter count@+72,
                                             *  emitters ptr@+76; emitters are 128 bytes) */
    tag_block    playlist;                  /* 0x1A0 */
};                                          /* 0x1AC (428 bytes) */

/* struct_game_globals — runtime game-state singleton pointed at by the
 * `game_globals` variable. Distinct DB type from the tag struct above. */
typedef struct struct_game_globals
{
    unsigned char map_loaded;               /* 0x00 */
    unsigned char active;                   /* 0x01 */
    unsigned char players_are_double_speed; /* 0x02 */
    unsigned char map_load_in_progress;     /* 0x03 */
    float         loading_progress;         /* 0x04 */
    game_options  options;                  /* 0x08 */
} struct_game_globals;                      /* 276 bytes */

extern struct_game_globals *game_globals;
