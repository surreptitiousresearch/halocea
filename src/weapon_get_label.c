/* weapon_get_label @0x836D8750 — return the weapon definition's marker label string
 * (_weapon_definition.label @0x308+4 = tag data +780), or the empty string when the weapon index is
 * invalid. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/blam_data_globals.h"

extern char empty_string[];  /* shared empty string "" */

const char *weapon_get_label(int weapon_index)
{
    if ( weapon_index == -1 )
        return empty_string;
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    return definition->weapon.label;
}
