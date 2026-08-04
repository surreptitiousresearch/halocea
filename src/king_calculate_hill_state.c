/* king_calculate_hill_state @0x8382C338 — King-of-the-Hill per-tick hill state/scoring update.
 *
 * Walks every player datum once, testing king_globals.on_the_hill[player_index] to see who is
 * standing inside the hill this tick, then advances the hill's state machine (king_globals.hill_state)
 * and its control timer (king_globals.hill_controlled_count). There are two independent scoring modes:
 *
 *   Teams mode (game_engine_has_teams()): players on the hill are split by team via the player datum's
 *   team index (+0x20). If both a "team 0" and a "team non-zero" player are present the hill is contested
 *   (state 4); otherwise the sole controlling side accrues time (state 3 for the non-zero team, state 2
 *   for team 0), resetting the timer whenever control changes hands.
 *
 *   Free-for-all mode: more than one player on the hill is contested (state 4); exactly one player holds
 *   it (state 1) and accrues time only while the same player (king_globals.hill_previous_controller)
 *   keeps it; an empty hill resets everything.
 *
 * A control timer that crosses 300 fires _multiplayer_sound_hill_contested (control lost while ahead) on
 * reset, and reaching exactly 300 fires _multiplayer_sound_hill_controlled (control milestone). Both sounds
 * are replicated to the network.
 *
 * The team index is player_datum.team_index at offset +0x20. on_the_hill[] is indexed by the low 16 bits
 * of the current data_iterator index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/king_globals.h"
#include "headers/player_datum.h"
#include "headers/king_hill_state.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_has_teams(void);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);

void king_calculate_hill_state(void)
{
    data_iterator iterator;
    player_datum *player;
    int           count_on_foot = 0;
    int           count_controlling;
    int           controller; /* left uninitialised as in the original; set whenever a player is on the hill */
    int           new_state;
    int           new_count;

    if (game_engine_has_teams())
    {
        count_controlling = 0;

        data_iterator_new(&iterator, player_data);
        for (player = data_iterator_next(&iterator);
             player != 0;
             player = data_iterator_next(&iterator))
        {
            if (king_globals.on_the_hill[(unsigned __int16)iterator.index])
            {
                if (player->team_index) /* team index (+0x20) */
                    ++count_controlling;
                else
                    ++count_on_foot;
            }
        }

        if (count_controlling)
        {
            if (count_on_foot)
            {
                /* both teams on the hill — contested */
                king_globals.hill_state = king_hill_contested;
                if (king_globals.hill_controlled_count > 300)
                    game_engine_play_multiplayer_sound(_multiplayer_sound_hill_contested, 1u);
                king_globals.hill_controlled_count = 0;
                return;
            }

            new_state = king_hill_controlled_blue;
            new_count = (king_globals.hill_state == king_hill_controlled_blue) ? king_globals.hill_controlled_count + 1 : 0;
            goto apply_hill_state;
        }

        if (!count_on_foot)
        {
            king_globals.hill_state = king_hill_uncontrolled;
            king_globals.hill_controlled_count = 0;
            return;
        }

        new_state = king_hill_controlled_red;
        new_count = (king_globals.hill_state == king_hill_controlled_red) ? king_globals.hill_controlled_count + 1 : 0;
        goto apply_hill_state;
    }

    /* free-for-all mode */
    data_iterator_new(&iterator, player_data);
    if (!data_iterator_next(&iterator))
    {
        king_globals.hill_state = king_hill_uncontrolled;
        king_globals.hill_controlled_count = 0;
        king_globals.hill_previous_controller = -1;
        return;
    }

    do
    {
        if (king_globals.on_the_hill[(unsigned __int16)iterator.index])
        {
            controller = iterator.index;
            ++count_on_foot;
        }
        player = data_iterator_next(&iterator);
    }
    while (player);

    if (count_on_foot <= 1)
    {
        if (count_on_foot)
        {
            if (king_globals.hill_state == king_hill_controlled && controller == king_globals.hill_previous_controller)
            {
                new_count = king_globals.hill_controlled_count + 1;
            }
            else
            {
                new_count = 0;
                king_globals.hill_previous_controller = controller;
            }
            new_state = king_hill_controlled;
            goto apply_hill_state;
        }

        /* empty hill */
        king_globals.hill_state = 0;
        king_globals.hill_controlled_count = 0;
        king_globals.hill_previous_controller = -1;
        return;
    }

    /* more than one player on the hill — contested */
    king_globals.hill_state = king_hill_contested;
    if (king_globals.hill_controlled_count > 300)
        game_engine_play_multiplayer_sound(_multiplayer_sound_hill_contested, 1u);
    king_globals.hill_controlled_count = 0;
    king_globals.hill_previous_controller = -1;
    return;

apply_hill_state:
    king_globals.hill_controlled_count = new_count;
    king_globals.hill_state = new_state;
    if (new_count == 300)
        game_engine_play_multiplayer_sound(_multiplayer_sound_hill_controlled, 1u);
}
