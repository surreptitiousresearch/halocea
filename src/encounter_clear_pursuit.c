/* encounter_clear_pursuit @0x83709CC0 — delete every pursuit datum chained off an encounter, walking the
 * next-pursuit links (pursuit_datum.next_pursuit_index) and freeing each. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/pursuit_datum.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void encounter_clear_pursuit(int encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    for ( int pursuit_index = encounter->first_pursuit_index; pursuit_index != -1;
          pursuit_index = encounter->first_pursuit_index )
    {
        encounter->first_pursuit_index =
            DATA_ARRAY_ELEMENT(pursuit_data, pursuit_datum, pursuit_index)->next_pursuit_index;
        datum_delete(pursuit_data, pursuit_index);
    }
}
