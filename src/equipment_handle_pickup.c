/* equipment_handle_pickup @0x837B5E60 — called when an equipment object is collected. Clears the
 * object's "can't be picked up" flag and plays the equipment definition's pickup sound. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/equipment_datum.h"
#include "headers/equipment_definition.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"

extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void equipment_handle_pickup(int equipment_index)
{
    /* object_header datum is 12 bytes; the object data pointer is the third dword (+2 words). */
    equipment_datum *equipment = ((equipment_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, equipment_index)->datum);

    /* The equipment definition is the loaded tag whose index is the object's first dword. */
    equipment_definition *definition = TAG_GET(equipment_definition, equipment->definition_index);

    /* 0x40 == item.flags bit 6 (item_flags $982827FBA02489028BA02B996F8FF3A9) */
    equipment->item.flags &= ~(1u << _item_part_of_respawn_system);

    int pickup_sound = definition->equipment.pickup_sound.index;  /* def+796 */
    if (pickup_sound != -1)
        unspatialized_impulse_sound_new(pickup_sound, 1.0f);
}
