/* weapon_can_be_fired @0x836D8F98 — decide whether a weapon may currently fire. Fully aged weapons
 * (age >= 1.0) can't. Outside a running game engine, or for weapons whose definition has no
 * magazines / a non-positive loaded-rounds maximum on the first magazine, firing is allowed
 * unconditionally. Otherwise it is allowed only while magazine 0 has loaded or total rounds. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);

uint8_t weapon_can_be_fired(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    if ( weapon->weapon.age >= 1.0f )
        return 0;
    if ( !game_engine_running() )
        return 1;
    if ( definition->weapon.magazines.count <= 0 )
        return 1;
    if ( ((weapon_magazine_definition *)definition->weapon.magazines.address)->rounds_loaded_maximum <= 0 )
        return 1;
    if ( weapon->weapon.magazines[0].rounds_loaded )
        return 1;
    if ( weapon->weapon.magazines[0].rounds_total )
        return 1;
    return 0;
}
