/* game_allegiance_create @0x83745988 — create or update the allegiance between two teams. If a record already
 * exists for either ordering it is overwritten; otherwise a new slot is appended (up to 8). The ally/friendly
 * bit-matrix entries for both orderings are set, and AI props are refreshed (status changed, not broken). */

#include <stdint.h>
#include "headers/game_allegiance.h"
#include "headers/bit_vector.h"

extern void ai_handle_allegiance_status_changed(int16_t team1_index, int16_t team2_index, uint8_t broken, uint8_t permanently_broken);

void game_allegiance_create(int16_t team1_index, uint8_t team1_suspicious, int16_t team2_index, uint8_t team2_suspicious, int16_t incident_threshold, int16_t incident_decay_time, uint8_t allegiance_requires_communication)
{
    struct_game_allegiance_globals *globals = game_allegiance_globals;
    game_allegiance *allegiance = globals->allegiances;
    int16_t allegiance_count = globals->allegiance_count;
    int index = 0;

    if ( allegiance_count > 0 )
    {
        do
        {
            int t1 = allegiance->team1_index;
            if ( t1 == team1_index && allegiance->team2_index == team2_index )
                break;
            if ( allegiance->team2_index == team1_index && t1 == team2_index )
                break;
            ++allegiance;
            index = (int16_t)(index + 1);
        }
        while ( index < game_allegiance_globals->allegiance_count );
    }

    if ( (int16_t)index >= allegiance_count && allegiance_count < 8 )
    {
        index = game_allegiance_globals->allegiance_count;
        game_allegiance_globals->allegiance_count = allegiance_count + 1;
    }

    if ( (int16_t)index < globals->allegiance_count )
    {
        game_allegiance *target = &globals->allegiances[(int16_t)index];
        target->team1_index = team1_index;
        target->team1_suspicious = team1_suspicious;
        target->team2_index = team2_index;
        target->team2_suspicious = team2_suspicious;
        target->incident_threshold = incident_threshold;
        target->incident_decay_time = incident_decay_time;
        target->current_incidents = 0;
        target->current_incident_decay_time = 0;
        target->requires_communication = allegiance_requires_communication;
        target->currently_broken = 0;

        if ( team1_index < 10 && team2_index < 10 )
        {
            int ab = 10 * team1_index + team2_index;
            int ba = 10 * target->team2_index + target->team1_index;
            BIT_VECTOR_SET_FLAG(globals->ally_bitvector, ab);
            BIT_VECTOR_SET_FLAG(globals->ally_bitvector, ba);
            int fab = 10 * target->team1_index + target->team2_index;
            int fba = 10 * target->team2_index + target->team1_index;
            BIT_VECTOR_SET_FLAG(globals->friendly_bitvector, fab);
            BIT_VECTOR_SET_FLAG(globals->friendly_bitvector, fba);
        }

        target->status_changed = 1;
        ai_handle_allegiance_status_changed(target->team1_index, target->team2_index, 0, 0);
        target->status_changed = 0;
    }
}
