#pragma once
/* player_ui_player_data / player_ui_globals_t — compatibility aliases onto the DB-authoritative
 * player-UI globals (player_ui_globals.h). The former minimal view mis-modeled profile_index at
 * offset 0; the DB _player_data slot carries it at 0x7BC, which player_data[0].profile_index now
 * resolves to correctly. */

#include "player_ui_globals.h"

typedef _player_data player_ui_player_data;
typedef _player_ui_globals player_ui_globals_t;
