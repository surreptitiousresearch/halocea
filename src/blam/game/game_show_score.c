/* game_show_score @0x8374DA00 — tail-call forwarder (`mr r7, r6; li r6, -1; b game_show_score_extended`;
 * confirmed via disasm, not a bare blr). Forwards to the extended variant with message_data forced to -1. */

#include <stdint.h>

extern void game_show_score_extended(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

void game_show_score(int recipient_player_index, int subject_player_index, int message, uint8_t should_replicate)
{
    game_show_score_extended(recipient_player_index, subject_player_index, message, -1, should_replicate);
}
