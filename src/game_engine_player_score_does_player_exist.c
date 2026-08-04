#include <stdint.h>
/* game_engine_player_score_does_player_exist @0x8374FFC0 — true when the player has an allocated score slot. */

extern int game_engine_player_score_get_slot_index_for_player(int player_index);

uint8_t game_engine_player_score_does_player_exist(int player_index)
{
    return game_engine_player_score_get_slot_index_for_player(player_index) != -1;
}
