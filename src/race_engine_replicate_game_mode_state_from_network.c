/* race_engine_replicate_game_mode_state_from_network @0x8382B308 — (client) apply a replicated Race game
 * mode state: incremental decodes go into the live race_globals (against the baseline), then the baseline
 * is resynced from the live struct; stateless decodes go straight into the baseline. On a successful
 * decode (result == 1), the live struct is (re)synced from the baseline the same way.
 *
 * SHIPPED QUIRK, disasm-verified: each "resync" is a leftover 64-byte doubleword-unrolled copy starting at
 * race_globals_type+0x80 (covering lap_bit_vector[15], rally_flag, and team_laps[0..13]), followed by
 * explicit memcpy of first_flag[16] and lap_bit_vector[16] (which overwrite the manual copy's
 * lap_bit_vector[15] portion) and explicit scalar copies of lap_completed_value/rally_flag (which
 * overwrite its rally_flag portion). Net effect: first_flag, lap_bit_vector, lap_completed_value, and
 * rally_flag are correctly synced; team_laps[0..13] is incidentally synced too, but team_laps[14..15] and
 * all of persistent_team_score are never touched by this function. Reproduced verbatim, in original
 * instruction order, rather than "cleaned up" into the equivalent minimal set of copies. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/race_globals.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern unsigned __int8 message_delta_processor_decode_incremental(void *destination_data, void *baseline_data,
        const message_delta_processor_header *header, unsigned __int8 allow_empty_body);
extern void *memcpy(void *destination, const void *source, unsigned int size);

static void resync(race_globals_type *destination, const race_globals_type *source)
{
    /* recovered: (char*)dst+0x80 -> &dst->lap_bit_vector[15]; preserved 0x40 byte-span quirk
       spanning lap_bit_vector[15], rally_flag, team_laps[0..13] */
    memcpy(&destination->lap_bit_vector[15], &source->lap_bit_vector[15], 0x40u);
    memcpy(destination->first_flag, source->first_flag, sizeof(destination->first_flag));
    memcpy(destination->lap_bit_vector, source->lap_bit_vector, sizeof(destination->lap_bit_vector));
    destination->lap_completed_value = source->lap_completed_value;
    destination->rally_flag = source->rally_flag;
}

void race_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    unsigned __int8 decoded;

    if ( header->decoding_information->mode )
    {
        decoded = message_delta_processor_decode_incremental(&race_globals, &race_globals_baseline, header, 0);
        resync(&race_globals_baseline, &race_globals);
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&race_globals_baseline, header);
    }

    if ( decoded == 1 )
        resync(&race_globals, &race_globals_baseline);
}
