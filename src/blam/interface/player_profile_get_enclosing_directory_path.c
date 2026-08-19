/* player_profile_get_enclosing_directory_path @0x83765330 — thunk to saved_game_file_get_path_to_enclosing_directory. */
#include <stdint.h>

extern uint8_t saved_game_file_get_path_to_enclosing_directory(int index, char *full_path);
/* attested uint8_t (was int): pure tail-call thunk, return type == callee's uint8_t */
uint8_t player_profile_get_enclosing_directory_path(int player_profile_index, char *full_path)
{
    return saved_game_file_get_path_to_enclosing_directory(player_profile_index, full_path);
}
