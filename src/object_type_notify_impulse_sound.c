/* object_type_notify_impulse_sound @0x83702420 — notify every part definition (super-type plus sub-types)
 * registered for an object's type that an impulse sound played. The object's type code lives at object
 * datum +180. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_type_notify_impulse_sound(int object_index, int sound_definition_index, int impulse_sound_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type = object_type_definitions[object->object.type];
    for ( int i = 0; type->part_definitions[i]; ++i )
    {
        void (*notify_impulse_sound)(int, int, int) = type->part_definitions[i]->notify_impulse_sound;
        if ( notify_impulse_sound )
            notify_impulse_sound(object_index, sound_definition_index, impulse_sound_index);
    }
}
