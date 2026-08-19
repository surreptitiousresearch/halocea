/* game_engine_get_place @0x83749000 — compute a player's (or team's) standing among all scoring participants
 * for the current game engine. Using the engine's get_score callback, it counts how many distinct participants
 * out-score the queried one (that count is the "place"), and sets flags describing the situation: tie at this
 * score, all participants tied, only two participants, and team-mode. In team mode participants are deduped by
 * team so each team counts once. Returns the packed game_engine_place {flags, place}.
 *
 * DEVIATION: the DB prototype (2 args, by-value return) is authoritative; the decompiler mismodeled it as a
 * __return_ptr `retstr` plus a phantom third arg. Disasm (clrlslwi r3 for the player record; lwz r3,var_80 to
 * return) shows r3 = player_index and r4 = score_type, the return struct packed in one register (flags at
 * offset 0, place at offset 2). The first get_score call is rendered arg-less by the decompiler but takes
 * (player_index, score_type) like the second (its arg registers are still live). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_engine.h"
#include "headers/game_engine_place.h"
#include "headers/game_engine_place_flags.h"
#include "headers/get_score_type.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


game_engine_place game_engine_get_place(int player_index, get_score_type score_type)
{
    int16_t place = 0;
    char all_scores_equal = 1;
    char tie_at_my_score = 0;
    int participant_count = 1;

    if ( game_engine->get_score )
    {
        player_datum *my_record = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int counted_team_mask = 0;
        int my_score = game_engine->get_score(player_index, score_type);

        data_iterator iterator;
        data_iterator_new(&iterator, player_data);
        for ( player_datum *entry = data_iterator_next(&iterator); entry; entry = data_iterator_next(&iterator) )
        {
            char count_this;
            if ( score_type == _get_score_team )
                count_this = my_record->team_index != entry->team_index;
            else
                count_this = player_index != iterator.index;

            if ( count_this && score_type == _get_score_team )
            {
                int team_bit = 1 << entry->team_index;
                if ( (team_bit & counted_team_mask) != 0 )
                {
                    count_this = 0;
                }
                else
                {
                    counted_team_mask |= team_bit;
                    count_this = 1;
                }
            }

            if ( count_this )
            {
                int other_score = game_engine->get_score(iterator.index, score_type);
                ++participant_count;
                if ( my_score != other_score )
                    all_scores_equal = 0;
                if ( other_score <= my_score )
                {
                    if ( other_score == my_score )
                        tie_at_my_score = 1;
                }
                else
                {
                    ++place;
                }
            }
        }
    }

    int16_t flags;
    if ( tie_at_my_score )
        flags = (score_type != _get_score_team ? 0 : (1 << _place_team)) | (1 << _place_tied);
    else
        flags = score_type != _get_score_team ? 0 : (1 << _place_team);
    if ( ((uint8_t)all_scores_equal & (uint8_t)tie_at_my_score) != 0 )
        flags |= 1 << _place_all_tied;
    else
        flags &= ~(1 << _place_all_tied);
    if ( participant_count == 2 )
        flags |= 1 << _place_two_groups;
    else
        flags &= ~(1 << _place_two_groups);

    game_engine_place result;
    result.flags = flags;
    result.place = place;
    return result;
}
