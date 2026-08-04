/* hcex_game_state_set_saved_game_valid @ 0x83684980
   Marks the current game-state arena as holding a valid saved game. */

#include "../headers/game_state_globals.h"

extern "C" void hcex_game_state_set_saved_game_valid(unsigned char value)
{
    game_state_globals.saved_game_valid = value;
}
