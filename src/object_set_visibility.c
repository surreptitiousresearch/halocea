/* object_set_visibility @ 0x836EF1F0 — show or hide an object. For objects with a render model
 * (def+52 != -1), toggling visibility also (dis)connects the object's lights: becoming visible
 * reconnects (object_connect_lights(idx, 0, 1)), becoming invisible disconnects (idx, 1, 0). The
 * invisible state is flag bit 1 at object_data+16 (set = invisible); the header's visible bit (0x2 in
 * byte[2]) mirrors it. A hidden object with no render model still updates its flags. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_flags.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


extern void object_connect_lights(int object_index, uint8_t disconnect, uint8_t reconnect);

void object_set_visibility(int object_index, uint8_t visible_flag)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;
    _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);

    if ( object_definition->model.index != -1 )
    {
        if ( (object_data->object.flags & (1u << _object_invisible_bit)) != 0 )
        {
            /* currently invisible */
            if ( visible_flag )
                object_connect_lights(object_index, 0, 1);  /* reconnect */
        }
        else
        {
            /* currently visible */
            if ( !visible_flag )
                object_connect_lights(object_index, 1, 0);  /* disconnect */
        }
    }

    if ( !visible_flag || object_definition->model.index != -1 )
    {
        unsigned int flags = object_data->object.flags;
        unsigned int new_flags = flags | (1u << _object_invisible_bit);
        if ( visible_flag )
            new_flags = flags & ~(1u << _object_invisible_bit);
        object_data->object.flags = new_flags;
        if ( visible_flag )
            object_header->flags |= (1u << _object_header_visible_bit);
        else
            object_header->flags &= ~(1u << _object_header_visible_bit);
    }
}
