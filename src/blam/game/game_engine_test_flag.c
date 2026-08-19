/* game_engine_test_flag @0x83749248 */
#include "headers/game_engine.h"

int game_engine_test_flag(int flag)
{
    if ( game_engine && game_engine->game_engine_test_flag )
        return game_engine->game_engine_test_flag(flag);
    return 0;
}
