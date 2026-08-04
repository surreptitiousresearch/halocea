/* encounters_create_for_new_map @0x8370E428 — for a freshly loaded map, instantiate every scenario encounter
 * that is not flagged manual-start (flags bit 0). Called from ai_place. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/encounter_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/encounter_flags.h"
#include "headers/blam_data_globals.h"


extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern encounter_datum *encounter_iterator_next(encounter_iterator *iterator);
extern void encounter_create(int encounter_index, int16_t desired_platoon_index, int16_t desired_squad_index);

void encounters_create_for_new_map(void)
{
    scenario *scenario_ptr = global_scenario;
    encounter_definition *encounters = (encounter_definition *)scenario_ptr->ai_encounters.address;
    encounter_iterator iterator;
    encounter_datum *encounter;

    if ( ai_globals->ai_initialized_for_map )
    {
        data_iterator_new(&iterator.encounter_iterator, encounter_data);
        iterator.active_only = 0;
    }

    /* bit 0 (_encounter_not_initially_placed) is the "manual start" flag: skip those at map load. */
    for ( encounter = encounter_iterator_next(&iterator); encounter; encounter = encounter_iterator_next(&iterator) )
    {
        if ( (encounters[(unsigned __int16)iterator.index].flags & (1u << _encounter_not_initially_placed_bit)) == 0 )
            encounter_create(iterator.index, -1, -1);
    }
    /* Returns nothing: r3 at blr is residue (NULL from the loop-exiting encounter_iterator_next); 0 callers. */
}
