#pragma once
#include <stdint.h>
/* _single_player_level_data — one campaign level's completion record (single_player_level_data[10]).
 * DB layout via types_members. */

typedef struct _single_player_level_data
{
    char            *scenario_tag_path;  /* 0x00 */
    uint8_t  level_completed[4]; /* 0x04 — per-difficulty completion flags */
} _single_player_level_data;

#ifdef __cplusplus
extern "C" {
#endif

extern _single_player_level_data single_player_level_data[10];

#ifdef __cplusplus
}
#endif
