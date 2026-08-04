/* object_connect_lights @ 0x836EDF48 — disconnect and/or reconnect the dynamic lights an object carries
 * to the structure BSP (e.g. when it moves, appears, or disappears). Only runs for objects flagged as
 * having lights (0x100). Iterates the definition's attachments block; a slot whose attachment_type is 0
 * (light) with attachment_indices[i] != -1 holds a connected light object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern void light_disconnect_from_map(uint16_t light_index);
extern void light_reconnect_to_map(uint16_t light_index);

void object_connect_lights(int object_index, uint8_t disconnect, uint8_t reconnect)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;

    if ( (object_data->object.flags & (1u << _object_has_attached_lights_bit)) != 0 )
    {
        _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);
        if ( object_definition->attachments.count > 0 )
        {
            __int16 i = 0;
            do
            {
                if ( !object_data->object.attachment_types[i] )
                {
                    int light_index = object_data->object.attachment_indices[i];
                    if ( light_index != -1 )
                    {
                        if ( disconnect )
                            light_disconnect_from_map(light_index);
                        if ( reconnect )
                            light_reconnect_to_map(object_data->object.attachment_indices[i]);
                    }
                }
                i = (__int16)(i + 1);
            }
            while ( i < object_definition->attachments.count );
        }
    }
}
