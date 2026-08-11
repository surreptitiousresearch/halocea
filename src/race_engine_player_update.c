/* race_engine_player_update @0x8382B790 — race engine: per-player per-tick update. Always sends the
 * generic engine state message (22). If the player has a controlled unit, scoring is allowed, and this is
 * a server (game_connection() == _game_connection_network_server), search for a nearby netgame flag (type 3) around the unit's bounding
 * sphere — using the unit's own position/search radius when it's not seated in a vehicle, or the parent
 * (vehicle) object's position/a wider radius when it is — and touch it if one is found.
 *
 * DEVIATION: the decompiler renders the search's `type`/`team` arguments as two locals with bogus
 * pointer-ish types (one holding a stale code-segment address, the other left unset on one branch);
 * disasm confirms both are just register-retained integer constants unrelated to those symbols — `type=3`
 * comes from an earlier `li r6, 3` (part of the object-header index math) whose register the compiler left
 * live and reused, and `team=-1` is set immediately before the call — reproduced here as the literal
 * constants find_netgame_flag actually receives. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/game_connection.h"
#include "headers/game_engine_message.h"
#include "headers/netgame_flag_type.h"
#include "headers/blam_data_globals.h"


extern void game_engine_state_message(int player_index, int message, int message_data);
extern uint8_t game_engine_can_score(void);
extern int16_t game_connection(void);
extern int find_netgame_flag(real_point3d *location, float distance, float height_delta, int16_t type, int16_t team);
extern void race_touch_flag(int player_index, int netgame_flag_index);

void race_engine_player_update(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    game_engine_state_message(player_index, game_engine_message_show_score, player_index);

    if ( player->unit_index != -1 && game_engine_can_score() && game_connection() == _game_connection_network_server )
    {
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;

        real_point3d *location;
        float distance;
        float height_delta;

        if ( unit->object.parent_object_index == -1 )
        {
            location = &unit->object.bounding_sphere_center;
            distance = 1.5f;
            height_delta = 0.6f;
        }
        else
        {
            object_datum *vehicle = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit->object.parent_object_index)->datum;
            location = &vehicle->object.bounding_sphere_center;
            distance = 2.5f;
            height_delta = 0.0f;
        }

        int netgame_flag = find_netgame_flag(location, distance, height_delta, _netgame_flag_race_track, -1);
        if ( netgame_flag != -1 )
            race_touch_flag(player_index, netgame_flag);
    }
}
