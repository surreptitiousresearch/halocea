/* actor_get_weapon_definition @0x837B79C8 — return the weapon tag definition for the weapon the actor is
 * currently holding, or null if it has none. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"

extern int actor_get_weapon(uint16_t actor_index);

weapon_definition *actor_get_weapon_definition(int actor_index)
{
    int weapon = actor_get_weapon(actor_index);
    if ( weapon == -1 )
        return 0;
    return TAG_GET(weapon_definition, *((_DWORD *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum));
}
