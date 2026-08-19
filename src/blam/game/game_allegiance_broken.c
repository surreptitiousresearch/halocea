/* game_allegiance_broken @0x837456E8 — set an allegiance's broken state and update the global team-relationship
 * bit matrices for both ordered team pairs. The ally_bitvector bit is cleared when the break is permanent and
 * set otherwise; the friendly_bitvector bit is cleared while currently broken and set otherwise. Marks the
 * record changed and notifies the AI. No-op unless the state actually changes (or the break is permanent). */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/game_allegiance.h"
#include "headers/blam_data_globals.h"

extern void ai_handle_allegiance_status_changed(int16_t team1_index, int16_t team2_index, uint8_t broken, uint8_t permanently_broken);

static void set_bit(unsigned int *bitvector, int bit, int set)
{
    if ( set )
        BIT_VECTOR_SET_FLAG(bitvector, bit);
    else
        BIT_VECTOR_CLEAR_FLAG(bitvector, bit);
}

void game_allegiance_broken(game_allegiance *allegiance, uint8_t currently_broken,
                            uint8_t permanently_broken)
{
    if ( !permanently_broken && allegiance->currently_broken == currently_broken )
        return;

    int team1_index = allegiance->team1_index;
    allegiance->currently_broken = currently_broken;

    if ( team1_index < 10 && allegiance->team2_index < 10 )
    {
        struct_game_allegiance_globals *globals = game_allegiance_globals;
        int forward = 10 * allegiance->team1_index + allegiance->team2_index;
        int reverse = 10 * allegiance->team2_index + allegiance->team1_index;

        set_bit(globals->ally_bitvector, forward, !permanently_broken);
        set_bit(globals->ally_bitvector, reverse, !permanently_broken);
        set_bit(globals->friendly_bitvector, forward, !currently_broken);
        set_bit(globals->friendly_bitvector, reverse, !currently_broken);
    }

    allegiance->status_changed = 1;
    ai_handle_allegiance_status_changed(allegiance->team1_index, allegiance->team2_index,
                                        currently_broken, permanently_broken);
}
