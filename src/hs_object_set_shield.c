/* hs_object_set_shield @0x837F8300 — set an object's current shield (object data +0xE4) to a fraction [0,1]
 * of its maximum shield (object data +0xDC). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


void hs_object_set_shield(int object_index, float shield)
{
    if ( object_index == -1 )
        return;
    if ( shield < 0.0f )
        shield = 0.0f;
    else if ( shield > 1.0f )
        shield = 1.0f;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_data->object.shield_vitality = object_data->object.maximum_shield_vitality * shield;
}
