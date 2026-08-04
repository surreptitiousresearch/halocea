/* game_engine_get_distance_rating_for_spawn @0x83747D0C — penalize a spawn point for being too close to other
 * players' objects. Starts at a perfect rating of 1.0 and, walking every player with a controlled object,
 * drives it down: any object within 0.25 units zeroes it; enemies (in team games) additionally require a
 * 2..5 unit gap and fade the rating in linearly across that band. */

#include "headers/scenario_player.h"
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

float game_engine_get_distance_rating_for_spawn(int player_index, const scenario_player *starting_location)
{
    unsigned __int8 teams = (game_engine != 0) ? global_variant.universal_variant.teams : 0;
    float rating = 1.0f;
    player_datum *spawning_player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *other_player = data_iterator_next(&iterator);
         other_player;
         other_player = data_iterator_next(&iterator))
    {
        int object_index = other_player->unit_index;   /* player datum + 52 */
        if (object_index == -1)
            continue;

        real_point3d origin;
        object_get_origin(object_index, &origin);

        float dx = starting_location->position.n[0] - origin.n[0];
        float dy = starting_location->position.n[1] - origin.n[1];
        float dz = starting_location->position.n[2] - origin.n[2];
        float distance = __fsqrts(dy * dy + (dx * dx + dz * dz));

        unsigned __int8 enemy = (other_player->team_index != spawning_player->team_index);   /* differing team_index (+32) */
        if (!teams || enemy || distance <= 0.25f)
        {
            if (distance >= 0.25f)
            {
                if (distance < 1.0f)
                    rating = rating * 0.1f;
            }
            else
            {
                rating = 0.0f;
            }

            if (enemy)
            {
                if (distance >= 2.0f)
                {
                    if (distance <= 5.0f)
                        rating = (distance - 2.0f) * rating * 0.33333334f;
                }
                else
                {
                    rating = 0.0f;
                }
            }
        }
    }

    return rating;
}
