/* hs_players @0x837F7798 — build and return a HaloScript object list containing every player's current unit
 * (player datum +0x34 unit index), skipping players with no unit. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int object_list_new(void);
extern int data_next_index(const data_array *data, int16_t index);
extern void object_list_add(int object_list_index, int object_index);

int hs_players(void)
{
    int object_list = object_list_new();
    for ( int i = data_next_index(player_data, -1); i != -1; i = data_next_index(player_data, i) )
    {
        int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, i)->unit_index;
        if ( unit_index != -1 )
            object_list_add(object_list, unit_index);
    }
    return object_list;
}
