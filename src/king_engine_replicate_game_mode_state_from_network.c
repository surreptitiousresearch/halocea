/* king_engine_replicate_game_mode_state_from_network @0x8382D188 — (client) apply a replicated King of the
 * Hill game mode state: incremental decodes seed the live king_globals.score[]/hill_id from the baseline
 * first (decode_incremental only touches fields present in the delta), then decode, then resync the
 * baseline from the now-updated live struct; stateless decodes go straight into the baseline. On a
 * successful decode, king_globals.hill_id is set and every score entry is rescaled by 30 from the
 * baseline; find_hill() runs if hill_id changed across the decode.
 *
 * DEVIATION: the decompiler renders the two 64-byte score[] copies via a pointer walk it (mis)labels
 * `race_globals.number_of_spawned_banshees`/`algn_8465AA90`; disasm confirms both loops actually address
 * king_globals/king_globals_baseline (an artifact of king_globals immediately following race_globals in
 * the data segment, and an unlabeled byte 16 before king_globals_baseline) — reproduced here as plain
 * king_globals.score[]/king_globals_baseline.score[] copies. The pre-`*30` copy of king_globals.score[]
 * from the baseline in the final block is provably dead (every element is overwritten by the `*30` loop
 * immediately after, nothing reads it in between) and is omitted. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/king_globals.h"
#include "headers/blam_data_globals.h"


extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_incremental(void *const destination_data, const void *const baseline_data, const message_delta_processor_header *const header, const uint8_t allow_empty_body);
extern void find_hill(void);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void king_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    uint8_t decoded;
    uint8_t hill_changed;
    int hill_id;

    if ( header->decoding_information->mode )
    {
        int old_hill_id = king_globals_baseline.hill_id;

        memcpy(king_globals.score, king_globals_baseline.score, sizeof(king_globals.score));
        king_globals.hill_id = old_hill_id;

        decoded = message_delta_processor_decode_incremental(&king_globals, &king_globals_baseline, header, 0);

        memcpy(king_globals_baseline.score, king_globals.score, sizeof(king_globals_baseline.score));
        king_globals_baseline.hill_id = king_globals.hill_id;
        hill_id = king_globals.hill_id;
        hill_changed = old_hill_id != hill_id;
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&king_globals_baseline, header);
        hill_id = king_globals_baseline.hill_id;
        hill_changed = 1;
    }

    if ( decoded == 1 )
    {
        king_globals.hill_id = hill_id;
        for ( int i = 0; i < 16; i++ )
            king_globals.score[i] = 30 * king_globals_baseline.score[i];

        if ( hill_changed )
            find_hill();
    }
}
