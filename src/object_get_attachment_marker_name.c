/* object_get_attachment_marker_name @ 0x836EBEA0 — return the marker name string an attachment is bound
 * to, from the object definition's attachments tag block (72-byte elements). Returns null if the
 * attachment index is out of range. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/blam_data_globals.h"


const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index)
{
    object_datum *object_data = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index))->datum;
    _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);

    if ( attachment_index < 0 || attachment_index >= object_definition->attachments.count )
        return 0;
    return ((object_attachment_definition *)object_definition->attachments.address)[attachment_index].marker_name;
}
