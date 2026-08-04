/* object_new @0x836F2188 — create a new object from a placement request, choosing its network datum role.
 * In a multiplayer server (game_connection == 2 == _game_connection_network_server), an object whose type has a
 * defined update-message type is created as a master datum; everything else (and all non-networked games) is
 * created as autonomous. */

#include <stdint.h>
#include "headers/object_placement_data.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/networked_datum_role.h"
#include "headers/game_connection.h"

extern int16_t game_connection(void);
typedef int message_delta_processor_message_definition_type; /* opaque message-type id (message_definition.h) */
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);

int object_new(object_placement_data *data)
{
    NetworkedDatumRole role = _networked_datum_autonomous;

    if ( game_connection() == _game_connection_network_server )
    {
        /* DEVIATION FIX: restored missing deref — DB reads **(_WORD **)(slot), the definition's type word */
        __int16 object_type = TAG_GET(_object_definition, data->definition_index)->type;
        role = object_type_get_update_message_type(object_type) != -1
                   ? _networked_datum_master
                   : _networked_datum_autonomous;
    }
    return object_new_with_datum_role_control(data, role);
}
