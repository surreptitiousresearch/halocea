/* game_engine_get_friendly_bonus @0x83747E78 — score how desirable a spawn point is by proximity to living
 * teammates. For each player on the same team that has a controlled object, the distance from the spawn point
 * to that object contributes a falloff weight (only when between 1 and 6 world units). The accumulated weight
 * is clamped to 3, scaled, and offset: bonus = min(weight, 3) * 3 + 1. */

#include "headers/scenario_player.h"
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern double pow(double base, double exponent);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

float game_engine_get_friendly_bonus(int player_index, const scenario_player *starting_location)
{
    float weight = 0.0f;
    player_datum *spawning_player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *other_player = data_iterator_next(&iterator);
    if (!other_player)
        return 0.0f * 3.0f + 1.0f;

    do
    {
        if (spawning_player->team_index == other_player->team_index)   /* same team_index (+32) */
        {
            int object_index = other_player->unit_index;   /* player datum + 52 */
            if (object_index != -1)
            {
                real_point3d origin;
                object_get_origin(object_index, &origin);

                float dx = starting_location->position.n[0] - origin.n[0];
                float dy = starting_location->position.n[1] - origin.n[1];
                float dz = starting_location->position.n[2] - origin.n[2];
                float distance = __fsqrts(dy * dy + (dx * dx + dz * dz));

                if (distance >= 1.0f && distance <= 6.0f)
                    weight = (float)pow(1.0f - (distance - 1.0f) * 0.2f, 0.6) + weight;
            }
        }
        other_player = data_iterator_next(&iterator);
    } while (other_player);

    if (weight <= 3.0f)
        return weight * 3.0f + 1.0f;
    return 3.0f * 3.0f + 1.0f;
}
