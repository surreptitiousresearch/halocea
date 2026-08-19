/* ai_get_race_from_team_index @0x836EA290 — map a game team index to its AI race bitmask. */

#include <stdint.h>
#include "headers/game_team.h"
#include "headers/actor_race.h"

int16_t ai_get_race_from_team_index(int16_t team_index)
{
    switch ( team_index )
    {
        case _game_team_player:   return _race_player;
        case _game_team_human:    return _race_human;
        case _game_team_covenant: return _race_covenant;
        case _game_team_flood:    return _race_flood;
        case _game_team_sentinel: return _race_sentinel;
        default:                  return _race_none;
    }
}
