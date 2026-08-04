#pragma once
/* animation_update_kind — selects which random stream drives a random animation permutation choice: the
 * game-state stream (deterministic, networked) or the local render-only stream. */

typedef enum animation_update_kind
{
    animation_update_kind_render_only        = 0,
    animation_update_kind_affects_game_state = 1,
} animation_update_kind;
