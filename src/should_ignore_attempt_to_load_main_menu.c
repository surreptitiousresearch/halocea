#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

int should_ignore_attempt_to_load_main_menu(void)
{
    return (player_control_globals->testing_for_action_flags >> _player_control_action_test_back_bit) & 1;
}
