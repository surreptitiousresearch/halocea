/* effects_stop_on_first_person_weapon @0x836E1018 — detach and free all effect locations owned by a given
 * local player's first-person weapon. Scans the effect data array; for each effect whose
 * local_player_index matches, it walks each of the effect definition's location lists
 * (location_datum_indices heads) and deletes every location datum that is marked attached
 * (node_designator: a real node attachment has bit 0x8000 set and is not 0xFFFF), unlinking it from the
 * singly-linked list. Finally clears the owner. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_location_datum.h"
#include "headers/effect_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);

void effects_stop_on_first_person_weapon(int16_t local_player_index)
{
    int index = data_next_index(effect_data, -1);
    if ( index == -1 )
        return;

    do
    {
        effect_datum *effect = DATA_ARRAY_ELEMENT(effect_data, effect_datum, index);
        effect_definition *definition =
            TAG_GET(effect_definition, effect->definition_index);

        if ( effect->local_player_index == local_player_index )
        {
            if ( definition->locations.count > 0 )
            {
                int location_index = 0;
                do
                {
                    int *list_node = &effect->location_datum_indices[location_index];
                    while ( *list_node != -1 )
                    {
                        effect_location_datum *location =
                            DATA_ARRAY_ELEMENT(effect_location_data, effect_location_datum, *list_node);
                        unsigned int designator = (uint16_t)location->node_designator;
                        if ( designator == 0xFFFF || (designator & 0x8000) == 0 )
                        {
                            list_node = &location->next_instance_location_index; /* keep: advance */
                        }
                        else
                        {
                            datum_delete(effect_location_data, *list_node);
                            *list_node = location->next_instance_location_index; /* unlink */
                        }
                    }
                    location_index = (int16_t)(location_index + 1);
                }
                while ( location_index < definition->locations.count );
            }
            effect->local_player_index = -1;
        }

        index = data_next_index(effect_data, index);
    }
    while ( index != -1 );
}
