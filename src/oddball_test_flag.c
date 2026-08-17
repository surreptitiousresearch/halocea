/* oddball_test_flag @0x838169A0 */
#include <stdint.h>
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);

uint8_t oddball_test_flag(int flag)
{
    return flag == 1 && game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_terminator;
}
