/* player_control_get_field_of_view @0x836DE798 — compute the camera field of view (radians) for a local
 * player. Defaults to 1.2217305 (70 deg). If the player has a unit, start from the unit definition's
 * camera_field_of_view; if the unit has a weapon in hand, let the weapon adjust it for the current zoom
 * level. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_control_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/blam_data_globals.h"

extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern float weapon_get_field_of_view(int weapon_index, float default_field_of_view, int16_t zoom_level);

float player_control_get_field_of_view(int16_t local_player_index)
{
    player_control *player = &player_control_globals->players[local_player_index];
    float field_of_view = 1.2217305f;

    int unit_index = player->unit_index;
    if ( unit_index != -1 )
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
        int weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
        field_of_view = definition->unit.camera_field_of_view;
        if ( weapon_index != -1 )
            field_of_view = weapon_get_field_of_view(weapon_index, field_of_view, player->desired_zoom_level);
    }
    return field_of_view;
}
