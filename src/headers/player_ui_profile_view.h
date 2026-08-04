#pragma once
/* player_ui_profile_view — compatibility aliases onto the DB-authoritative player-UI
 * layouts. The canonical types are _player_data (_player_data.h) and
 * _player_ui_globals (player_ui_globals.h); the previously-modeled local
 * slot/view bodies were superseded by the DB-verified layouts. Consumers keep
 * accessing player_ui_globals.player_data[i].profile / .profile_index. */

#include "player_ui_globals.h"

typedef _player_data player_ui_profile_slot;      /* 1988 bytes */
typedef _player_ui_globals player_ui_profile_view;
