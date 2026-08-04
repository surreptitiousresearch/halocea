#include <stdint.h>
#include "headers/ctf_message.h"

extern void game_show_score_team(int team, int message, uint8_t should_replicate);

void ctf_single_flag_what_is_up_message(int flag_team_index)
{
    game_show_score_team(flag_team_index % 2, ctf_message_you_are_on_defense, 1u);
    game_show_score_team((flag_team_index + 1) % 2, ctf_message_you_are_on_attack, 1u);
}
