/* object_pvs_set_object @0x836EC988 — force the potentially-visible-set to activate around the given object
 * (type 1), or clear the override when passed -1. */

#include "headers/object_globals.h"
#include "headers/blam_data_globals.h"
#include "headers/object_pvs_activation_type.h"


void object_pvs_set_object(int object_index)
{
    if ( object_index == -1 )
    {
        object_globals->pvs_activation_type = 0;
    }
    else
    {
        object_globals->pvs_activation.object_index = object_index; /* recovered: pvs_activation = object_index -> .object_index */
        object_globals->pvs_activation_type = _pvs_activation_object;
    }
}
