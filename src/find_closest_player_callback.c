/* find_closest_player_callback @0x83746690 — find_objects_from_point_vector add-object predicate for
 * find_closest_player_index: accept live, non-destroyed bipeds that aren't the requesting player. */

#include <stdint.h>
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);

uint8_t find_closest_player_callback(int object_index, int *custom_data)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if (object->object.flags & (1u << _object_invisible_bit))
        return 0;
    if (((1 << object->object.type) & object_mask_biped) == 0)
        return 0;
    if (object->object.damage_flags & (1u << _object_dead_bit))
        return 0;

    return player_index_from_unit_index(object_index) != *custom_data;
}
