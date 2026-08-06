/* encounter_get_actor_starting_location @0x8370A528 — choose a starting-location index for the next actor of a
 * squad. Preference order: a still-required location (picked at random and consumed), otherwise an unused
 * location (the unused set is refilled when exhausted but required ones remain). Returns -1 if none available.
 *
 * required_locations / unused_locations are the two parallel one-word bit vectors at the head of squad_datum,
 * indexed by starting-location index (squad_definition.starting_locations, +208).
 *
 * DEVIATION (signature): the DB prototype is 2-param — (unsigned __int16 encounter_index, __int64 spawn) —
 * because IDA fused r4:r5 into one 64-bit slot. Disasm 0x8370A564 uses r4 as the squad_definition index
 * (mulli r10, r4, 0xE8 = the 232-byte squad_definition stride), so the real ABI is r3=encounter_index,
 * r4=squad_index, r5=spawn. r5 is never read before being written (first write 0x8370A560), i.e. `spawn` is
 * genuinely dead in this routine; it is kept for call-site compatibility. */

#include <stdint.h>
#include "headers/squad_definition.h"
#include "headers/encounter_definition.h"
#include <string.h>
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int16_t encounter_get_actor_starting_location(uint16_t encounter_index, int squad_index, uint8_t spawn)
{
    (void)spawn;

    encounter_definition *encounter_def = &((encounter_definition *)global_scenario->ai_encounters.address)[encounter_index];
    squad_definition *squad = &((squad_definition *)encounter_def->squads.address)[squad_index];
    int location_count = squad->starting_locations.count;

    /* squad_datum index = encounter datum's squad base + squad_index */
    int squad_datum_index = (int16_t)(DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->squad_base + squad_index);
    squad_datum *runtime = &squad_array[squad_datum_index];

    /* Per-call "already assigned" bit vector. DEVIATION: sized [2], not [8]. The frame is 0x90 bytes with a
     * single local at +0x50 (IDA models it as one 0x40 blob, but 0x14 of that is the __savegprlr_27 save
     * area), and exactly 8 bytes are ever written to it — one `std` of zero at 0x8370A598. Nothing reads
     * past index 63 in practice either: squad_datum is 32 bytes (slwi r6, 5 @0x8370A58C) and its
     * required/unused banks are one word each, so location_count is bounded by 32 by the datum's own
     * layout — the same bound the refill memset below relies on to stay inside unused_locations. */
    uint32_t taken[2];
    taken[0] = 0;
    taken[1] = 0;

    int chosen = -1;

    /* --- pass 1: required locations --- */
    int16_t required_available = 0;
    if ( location_count > 0 )
    {
        for ( int16_t i = 0; i < location_count; ++i )
        {
            if ( BIT_VECTOR_TEST_FLAG(runtime->required_locations, i) && !BIT_VECTOR_TEST_FLAG(taken, i) )
                ++required_available;
        }
    }
    if ( required_available > 0 )
    {
        uint32_t *seed = get_global_random_seed_address();
        int16_t pick = seed_random_range(seed, 0, required_available);
        int count = squad->starting_locations.count;
        if ( count > 0 )
        {
            int16_t index = 0;
            do
            {
                if ( BIT_VECTOR_TEST_FLAG(runtime->required_locations, index) && !BIT_VECTOR_TEST_FLAG(taken, index) )
                {
                    if ( !pick )
                    {
                        chosen = index;
                        /* disasm 0x8370A698-0x8370A6B4 clears the bit in *(runtime + word) and in
                         * *(runtime + word + 4); squad_datum puts unused_locations at +4, immediately after
                         * required_locations, so the second store is that bank — spelled explicitly here
                         * rather than as required_locations[word + 1] pointer arithmetic. */
                        BIT_VECTOR_CLEAR_FLAG(runtime->required_locations, index);
                        BIT_VECTOR_CLEAR_FLAG(runtime->unused_locations, index);
                        break;
                    }
                    --pick;
                }
                ++index;
            }
            while ( index < count );
        }
    }
    if ( (int16_t)chosen != -1 )
        return chosen;

    /* --- pass 2: unused locations, refilling the unused set if it is empty but required ones remain --- */
    int16_t unused_available;
    uint8_t refilled;
    do
    {
        int count = squad->starting_locations.count;
        uint8_t has_non_unused = 0;
        refilled = 0;
        unused_available = 0;
        if ( count > 0 )
        {
            for ( int16_t j = 0; j < count; ++j )
            {
                if ( !BIT_VECTOR_TEST_FLAG(taken, j) )
                {
                    if ( BIT_VECTOR_TEST_FLAG(runtime->unused_locations, j) )
                        ++unused_available;
                    else
                        has_non_unused = 1;
                }
            }
        }
        if ( has_non_unused && !unused_available )
        {
            memset(runtime->unused_locations, -1, sizeof(uint32_t) * BIT_VECTOR_SIZE_IN_LONGS(count));
            refilled = 1;
        }
    }
    while ( refilled );

    if ( unused_available <= 0 )
        return chosen;

    uint32_t *seed = get_global_random_seed_address();
    int16_t pick = seed_random_range(seed, 0, unused_available);
    int count = squad->starting_locations.count;
    if ( count <= 0 )
        return chosen;

    int16_t index = 0;
    while ( 1 )
    {
        if ( BIT_VECTOR_TEST_FLAG(runtime->unused_locations, index) && !BIT_VECTOR_TEST_FLAG(taken, index) )
        {
            if ( !pick )
                break;
            --pick;
        }
        ++index;
        if ( index >= count )
            return chosen;
    }
    BIT_VECTOR_CLEAR_FLAG(runtime->unused_locations, index);
    return index;
}
