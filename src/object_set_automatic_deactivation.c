/* object_set_automatic_deactivation @0x836EC138 — set/clear the object's automatic-deactivation flag
 * (header flags bit 0x40). When enabling, if the object is not parented and has no cluster
 * (location.cluster_index == 0xFFFF), it also clears the "active" bit (0x01). When disabling, if the
 * object is currently inactive it is reactivated via object_activate. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"

extern void object_activate(int object_index);

void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation)
{
    object_header_datum *header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    if ( automatic_deactivation )
        header->flags |= (1u << _object_header_automatically_deactivate_bit);
    else
        header->flags &= ~(1u << _object_header_automatically_deactivate_bit);

    if ( automatic_deactivation )
    {
        if ( object->object.parent_object_index == -1
          && (uint16_t)object->object.location.cluster_index == 0xFFFF )
        {
            if ( (header->flags & (1u << _object_header_active_bit)) != 0 )
                header->flags &= ~(1u << _object_header_active_bit);
        }
    }
    else if ( (header->flags & (1u << _object_header_active_bit)) == 0 )
    {
        object_activate(object_index);
    }
}
