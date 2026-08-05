/* encounter_get_actor_starting_location @0x8370A528 — choose a starting-location index for the next actor of a
 * squad. Preference order: a still-required location (picked at random and consumed), otherwise an unused
 * location (the unused set is refilled when exhausted but required ones remain). Returns -1 if none available.
 *
 * required_locations / unused_locations are parallel bitmasks of length location_count (squad +208). The
 * `spawn` parameter is accepted for signature compatibility but unused by this routine. */

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

    uint32_t taken[8];          /* per-call "already assigned" bitmask; only the first two words are
                                 * initialized (a single 8-byte `std` of zero in the disassembly) */
    taken[0] = 0;
    taken[1] = 0;

    int chosen = -1;

    /* --- pass 1: required locations --- */
    int16_t required_available = 0;
    if ( location_count > 0 )
    {
        for ( int i = 0; i < location_count; i = (int16_t)(i + 1) )
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
            int index = 0;
            do
            {
                if ( BIT_VECTOR_TEST_FLAG(runtime->required_locations, index) && !BIT_VECTOR_TEST_FLAG(taken, index) )
                {
                    if ( !pick )
                    {
                        unsigned int *required_word = &runtime->required_locations[index >> 5];
                        chosen = index;
                        *required_word &= ~(1 << (index & 31));
                        required_word[1] &= ~(1 << (index & 31)); /* also clear from the adjacent unused_locations bank */
                        break;
                    }
                    --pick;
                }
                index = (int16_t)(index + 1);
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
            for ( int j = 0; j < count; j = (int16_t)(j + 1) )
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
            memset(runtime->unused_locations, -1, 4 * BIT_VECTOR_SIZE_IN_LONGS(count));
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

    int index = 0;
    while ( 1 )
    {
        if ( BIT_VECTOR_TEST_FLAG(runtime->unused_locations, index) && !BIT_VECTOR_TEST_FLAG(taken, index) )
        {
            if ( !pick )
                break;
            --pick;
        }
        index = (int16_t)(index + 1);
        if ( index >= count )
            return chosen;
    }
    BIT_VECTOR_CLEAR_FLAG(runtime->unused_locations, index);
    return index;
}
