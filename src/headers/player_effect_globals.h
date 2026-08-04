#pragma once
/* player_effect_globals — per-local-player screen/camera feedback effects (player_effect.c). Each
 * player_effect_datum (236 bytes) holds the directional vectors plus screen flash / camera impulse /
 * shake / continuous effect state. The full 532-byte definition (with the embedded screen_fade and
 * scripted_effect members and the global `player_effect_globals` pointer) lives in
 * player_effect_globals_definition.h. */

#include "player_effect_globals_definition.h"

/* Orphaned screen-fade scratch globals at 0x84429BF0-0x84429C04 (no surviving named
 * consumer; types from DB applied_types). Declared here as the closest subsystem home;
 * storage definitions live in src/data/. */
extern int   fade_function;    /* 0x84429BF0 */
extern float alpha_0;          /* 0x84429BF4 */
extern float green_1;          /* 0x84429BF8 */
extern int   fade_function_0;  /* 0x84429BFC */
extern float alpha_1;          /* 0x84429C00 */
extern float blue_1;           /* 0x84429C04 */
