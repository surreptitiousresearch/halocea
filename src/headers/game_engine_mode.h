#pragma once
/* game_engine_mode — multiplayer engine phase (game_engine.c).
 * enum : __int32, 4 bytes. */

typedef enum game_engine_mode
{
    game_engine_mode_active                   = 0,
    game_engine_mode_postgame_delay           = 1,
    game_engine_mode_postgame_rasterize_delay = 2,
    game_engine_mode_postgame_rasterize       = 3,
} game_engine_mode;
