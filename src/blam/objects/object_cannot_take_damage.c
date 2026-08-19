/* object_cannot_take_damage @0x836B17F8 — set the damage-immunity flag (object data +0x106 bit 0x800) on
 * every object in the list, making them invulnerable. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);

void object_cannot_take_damage(int object_list_index)
{
    int iterator[2];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        object_data->object.damage_flags |= (1u << _object_cannot_take_damage_bit);
    }
}
