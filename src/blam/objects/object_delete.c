/* object_delete @ 0x836F07F0 — request deletion of one object. The NetworkedDatumRole word at
 * object_data+4 (lwz r11,4(r6) @0x836F0828) selects who owns the delete:
 * 0 = _networked_datum_master — we own a replicated datum, so notify the network first;
 * 3 = _networked_datum_autonomous — never replicated, delete locally with no notification;
 * 1/2 = _networked_datum_puppet[_controlled_by_local_player] — remotely owned, so bail.
 * Then run the recursive teardown (children + this object).
 * Object datum: 12-byte header element, ->datum is the object data pointer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


extern void object_delete_to_network(int object_index);
extern void object_delete_initial_recursive(int object_index, uint8_t delete_siblings);

void object_delete(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;
    int network_simulation = object_data->object.datum_role;

    if ( network_simulation )
    {
        if ( network_simulation != 3 )
            return;
    }
    else
    {
        object_delete_to_network(object_index);
    }

    object_delete_initial_recursive(object_index, 0);
}
