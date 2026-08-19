/* game_engine_test_trait @0x83749280 */
#include "headers/game_engine.h"

int game_engine_test_trait(int player_index, int trait)
{
    if ( game_engine && game_engine->game_engine_test_trait )
        return game_engine->game_engine_test_trait(player_index, trait);
    return 0;
}
