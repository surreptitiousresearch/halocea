#pragma once
/* game_variant — the active multiplayer game variant (152 bytes). Only the fields read by
 * vehicle_update are modeled precisely; the engine-specific variant block is kept opaque. */

#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
#include "universal_variant.h"
#include "game_engine_variant.h"

typedef struct game_variant
{
    wchar_t           human_readable_game_description[24]; /* 0x00 */
    int               game_engine_index;     /* 0x30 */
    universal_variant universal_variant;     /* 0x34 */
    game_engine_variant game_engine_variant; /* 0x7C — engine-specific block (union of per-mode views) */
    uint16_t  flags;                 /* 0x94 */
} game_variant;                              /* 152 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern game_variant global_variant;

#ifdef __cplusplus
}
#endif
