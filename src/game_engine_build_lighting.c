/* game_engine_build_lighting @0x83746048 — derive the lighting/quality "scene complexity" flags for the
 * current multiplayer scene from the player count and a secondary count (territory-style netgame flags of
 * race-vehicle netgame flags (type 4) for the race engine, otherwise the number of vehicle objects). The
 * resulting bits in game_engine_globals.flags gate progressively heavier lighting as the scene gets busier
 * (bits 0/1 disable dynamic/integrated lights when busy; bits 2/3 mark the 5+/9+ player thresholds).
 *
 * DEVIATION: the reconstruction returned the last object_iterator_next result; that r3 is iterator
 * residue — the sole caller (game_engine_game_starting) ignores it. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/object_iterator.h"
#include "headers/game_variant.h"
#include "headers/game_engine_globals.h"
#include "headers/game_engine_flags.h"
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/game_engine_type.h"
#include "headers/object_type.h"
#include "headers/game_engine_vehicles.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);

void game_engine_build_lighting(void)
{
    int player_count = 0;
    int secondary_count = 0;

    object_iterator iterator;
    data_iterator_new((data_iterator *)&iterator, player_data);
    void *result;
    for (result = data_iterator_next((data_iterator *)&iterator); result;
         result = data_iterator_next((data_iterator *)&iterator))
        ++player_count;

    if (global_variant.universal_variant.vehicle_set.__s1.preset != _game_engine_vehicles_none)
    {
        if (global_variant.game_engine_index == game_engine_race)
        {
            int flag_count = global_scenario->netgame_flags.count;
            for (int i = 0; i < flag_count; i = (int16_t)(i + 1))
            {
                if (((scenario_netgame_flag *)global_scenario->netgame_flags.address)[i].type == _netgame_flag_race_vehicle)
                    ++secondary_count;
            }
            if (secondary_count > player_count)
                secondary_count = player_count;
        }
        else
        {
            object_iterator_new(&iterator, object_mask_vehicle, 0);
            for (result = object_iterator_next(&iterator); result; result = object_iterator_next(&iterator))
                ++secondary_count;
        }
    }

    int flags = game_engine_globals.flags;
    if ((player_count > 8 && secondary_count >= 2) || player_count >= 13)
    {
        flags |= (1u << _game_engine_disable_dynamic_light_bit);
        game_engine_globals.flags = flags;
    }
    if (player_count > 4 || secondary_count >= 4 || (flags & (1u << _game_engine_disable_dynamic_light_bit)) != 0)
    {
        flags |= (1u << _game_engine_disable_integrated_lights_bit);
        game_engine_globals.flags = flags;
    }
    if (player_count >= 5)
    {
        flags |= (1u << _game_engine_5_or_more_players_bit);
        game_engine_globals.flags = flags;
    }
    if (player_count >= 9)
        game_engine_globals.flags = flags | (1u << _game_engine_9_or_more_players_bit);
}
