/* ai_scripting_set_team @0x83772830 — change an encounter's team, reassign every actor of that encounter
 * to the new team, and refresh global team status. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/blam_data_globals.h"

extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void actor_set_team(int actor_index, int16_t team_index);
extern void ai_update_team_status(void);

void ai_scripting_set_team(int ai_index, int16_t team_index)
{
    DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index)->team_index = team_index;

    encounter_actor_iterator iterator;
    encounter_actor_iterator_new(&iterator, ai_index);
    while ( encounter_actor_iterator_next(&iterator) )
        actor_set_team(iterator.index, team_index);

    ai_update_team_status();
}
