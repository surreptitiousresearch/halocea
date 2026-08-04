#include "headers/game_engine.h"

int game_engine_test_flag(int flag)
{
    if ( game_engine && game_engine->game_engine_test_flag )
        return game_engine->game_engine_test_flag(flag);
    return 0;
}
