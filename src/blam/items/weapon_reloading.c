/* weapon_reloading @0x836D8DB8 — true if the weapon has at least one magazine and its first magazine is
 * currently in the reloading state (weapon_magazine.state == 1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/weapon_magazine_state.h"


uint8_t weapon_reloading(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    if ( definition->weapon.magazines.count > 0 )
        return weapon->weapon.magazines[0].state == _magazine_reloading;
    return 0;
}
