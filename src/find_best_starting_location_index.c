/* find_best_starting_location_index @0x836A9060 — choose a spawn point for a player by scoring each scenario
 * starting location and keeping the best. Each location's score is its game-engine rating weighted by a
 * random factor sqrt(rand[0,1)) (pow(x, 0.5)), so higher-rated spawns win but ties break randomly. Returns
 * the index of the best location, or -1 if none scored above zero. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_player.h"

extern float game_engine_get_starting_location_rating(int player_index, scenario_player *starting_location);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern double pow(double base, double exponent);

int16_t find_best_starting_location_index(int player_index)
{
    int16_t best_index = -1;
    int16_t index = 0;
    float best_score = 0.0f;

    int count = (int16_t)global_scenario->players.count;
    if (count <= 0)
        return best_index;

    int i = 0;
    while (1)
    {
        scenario *scenario = global_scenario;
        scenario_player *starting_location = nullptr;
        if (i >= 0 && i < scenario->players.count)
            starting_location = &((scenario_player *)scenario->players.address)[i];

        float rating = game_engine_get_starting_location_rating(player_index, starting_location);
        unsigned int *seed = get_global_random_seed_address();
        float weight = (float)pow((double)real_seed_random_range(seed, 0.0f, 1.0f), 0.5);
        float score = weight * rating;
        if (score > best_score)
        {
            best_index = index;
            best_score = score;
        }

        index = (int16_t)(i + 1);
        i = index;
        if (index >= count)
            break;
    }

    return best_index;
}
