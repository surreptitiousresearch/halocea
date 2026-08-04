#ifndef NETGAME_GAME_TYPE_CATEGORY_H
#define NETGAME_GAME_TYPE_CATEGORY_H

#include "game_engine_type.h"

/* Category codes used in scenario netgame-equipment game-type lists (see match_game_type.c).
 * Values 0..7 are plain game_engine_type entries; 12/13/14 are wildcard categories.
 * NO DB ENUM: exhaustive types_enum_values + headers_ref search found no compiled enum for
 * these categories (game_engine_type ends at 7). Names below are derived from the
 * code-proven semantics in match_game_type @0x8374CD78, not from a DB citation. */
enum netgame_game_type_category {
	_netgame_game_type_all_games = 12,            /* matches every engine type */
	_netgame_game_type_all_except_ctf = 13,       /* matches every type except ctf */
	_netgame_game_type_all_except_ctf_race = 14,  /* matches every type except ctf and race */
};

#endif /* NETGAME_GAME_TYPE_CATEGORY_H */
