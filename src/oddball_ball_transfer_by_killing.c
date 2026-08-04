#include "headers/game_variant.h"
#include "headers/oddball_ball_type.h"

extern game_variant *game_engine_get_variant(void);

int oddball_ball_transfer_by_killing(void)
{
    oddball_ball_type type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    if ( type <= _oddball_normal )
        return 0;
    if ( type > _oddball_terminator )
        return 0;
    return 1;
}
