/* equipment_definition_handle_pickup @0x837B5EC8 */
/* equipment_definition_handle_pickup 0x837B5EC8 — play an equipment's pickup sound when it is collected. The
 * pickup sound tag index lives at dword[199] (offset 796) of the equipment tag definition; -1 means no sound. */

#include "headers/global_tag_instances.h"
#include "headers/equipment_definition.h"

extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void equipment_definition_handle_pickup(int equipment_definition_index)
{
    equipment_definition *definition = TAG_GET(equipment_definition, equipment_definition_index);
    int pickup_sound = definition->equipment.pickup_sound.index;  /* def+796 */
    if ( pickup_sound != -1 )
        unspatialized_impulse_sound_new(pickup_sound, 1.0f);
}
