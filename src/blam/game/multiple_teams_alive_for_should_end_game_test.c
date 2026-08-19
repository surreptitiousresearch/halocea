/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* multiple_teams_alive_for_should_end_game_test @0x8374B6A8 — part of the game-end evaluation: decide
 * whether the surviving players still represent enough living teams to keep the game going.
 *
 * With more than one player in the game, an unlimited-lives non-LMS variant always passes (return 1).
 * Otherwise each player is classified alive/dead (an explicit alive flag, a still-pending respawn
 * state, odd-man-out exclusion, or lives remaining under a life cap), the distinct teams present are
 * tracked, and the moment two different living teams are seen the test passes. If only a single team
 * is even present it also passes; otherwise it fails (0).
 *
 * With one player it returns whether that player still has lives left (the single-player survival
 * check); with no players it returns 1.
 *
 * Player datum fields (now typed via player_datum): quit_out_of_game, unit_index (-1 == no unit /
 * awaiting respawn), team_index, statistics.deaths (compared against universal_variant.lives). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


extern int players_in_game(void);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator); /* DB: returns void* */
extern uint8_t game_engine_player_is_odd_man_out(int player_index);

uint8_t multiple_teams_alive_for_should_end_game_test(void)
{
    if (players_in_game() > 1)
    {
        if (global_variant.universal_variant.lives <= 0 && !global_variant.universal_variant.odd_man_out)
            return 1;

        int first_alive_team = -1;
        char saw_multiple_teams = 0;
        int first_team = -1;

        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        for (int *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator))
        {
            char alive;
            if (((player_datum *)player)->quit_out_of_game)
            {
                alive = 1;
            }
            else if (((player_datum *)player)->unit_index != -1)
            {
                alive = 1; /* respawn pending — still in the game */
            }
            else if (game_engine_player_is_odd_man_out(iterator.index))
            {
                alive = 0;
            }
            else
            {
                char out_of_lives = 0;
                if (global_variant.universal_variant.lives > 0)
                {
                    player_datum *entry = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);
                    if (entry->unit_index == -1)
                        out_of_lives = entry->statistics.deaths
                                       >= global_variant.universal_variant.lives;
                }
                alive = !out_of_lives;
            }

            int team = ((player_datum *)player)->team_index;
            if (first_team == -1)
                first_team = team;
            else if (first_team != team)
                saw_multiple_teams = 1;

            if (alive && team != first_alive_team)
            {
                if (first_alive_team != -1)
                    return 1;
                first_alive_team = team;
            }
        }

        if (!saw_multiple_teams)
            return 1;
        return 0;
    }
    else
    {
        int has_lives = 1;
        if (players_in_game() == 1)
        {
            data_iterator iterator;
            data_iterator_new(&iterator, player_data);
            while (data_iterator_next(&iterator))
            {
                char out_of_lives = 0;
                if (global_variant.universal_variant.lives > 0)
                {
                    player_datum *entry = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);
                    if (entry->unit_index == -1)
                        out_of_lives = entry->statistics.deaths
                                       >= global_variant.universal_variant.lives;
                }
                has_lives = !out_of_lives;
            }
        }
        return has_lives;
    }
}
