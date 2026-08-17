/* interface_get_weapon_hud_index @0x836A6630 */
/* interface_get_weapon_hud_index 0x836A6630 — resolve the weapon HUD tag index to display for the local
 * player being rendered, and report that weapon's flashlight/battery power through *flashlight_power. Returns
 * -1 when the HUD is hidden, the camera is in a non-first-person perspective (2/3), the player has no unit, or
 * no weapon HUD applies. When the player's own inventory slot is empty and the unit is riding in a parent
 * vehicle, it falls back to that vehicle's current weapon HUD -- but only if the unit's occupied seat is a
 * gunner seat (unit_seat.flags & _unit_seat_is_gunner_bit) -- and, failing that, to the default weapon HUD.
 *
 * The weapon HUD tag comes from weapon_definition.weapon.interface_definition.hud_interface.index; the seat
 * block is unit_definition.unit.seats.address indexed by the occupied seat. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/render_globals.h"
#include "headers/hud_scripted_globals.h"
#include "headers/hud_globals.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/weapon_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);

int interface_get_weapon_hud_index(float *flashlight_power)
{
    int player_index;
    int hud_index;
    float flashlight;
    player_datum *player;
    int16_t perspective;
    int weapon_index;
    unit_datum *unit_object_data;
    int16_t parent_seat_index;
    data_array *objects;
    int weapon_hud_index;
    int player_unit_index;

    player_index = local_player_get_player_index(render.local_player_index);
    hud_index = -1;
    flashlight = 0.0f;
    if ( player_index == -1 )
        goto done;

    player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    perspective = director_get_perspective(render.local_player_index);
    player_unit_index = player->unit_index;
    if ( !hud_scripted_globals
      || !hud_scripted_globals->show_hud
      || perspective == _director_perspective_neutral
      || perspective == _director_perspective_scripted
      || player_unit_index == -1 )
    {
        goto done;
    }

    weapon_index = unit_inventory_get_weapon(
        player_unit_index,
        ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit_index)->datum)->unit.current_weapon_index);

    if ( weapon_index == -1 )
    {
        unit_object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit_index)->datum;
        if ( unit_object_data->object.parent_object_index == -1 )
            goto done;
        parent_seat_index = unit_object_data->unit.parent_seat_index;
        if ( parent_seat_index == -1 )
            goto done;
        {
            /* [186] (byte 744) = unit_definition.unit.seats.address; 284-byte stride = sizeof(unit_seat);
             * & 8 = seat flags bit 3 = _unit_seat_is_gunner_bit. Only gunner seats show the vehicle weapon HUD. */
            unit_definition *parent_definition = TAG_GET(unit_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object_data->object.parent_object_index)->datum->definition_index);
            unit_seat *parent_seat = (unit_seat *)parent_definition->unit.seats.address;
            if ( (parent_seat[parent_seat_index].flags & (1u << _unit_seat_is_gunner_bit)) == 0 )
                goto done;
        }
        weapon_index = unit_inventory_get_weapon(
            unit_object_data->object.parent_object_index,
            ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object_data->object.parent_object_index)->datum)->unit.current_weapon_index);
        objects = object_header_data;
    }
    else
    {
        objects = object_header_data;
        flashlight = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit_index)->datum)->unit.integrated_night_vision_power;
    }

    if ( weapon_index != -1 )
    {
        /* [291] (byte 1164) = weapon_definition.weapon.interface_definition.hud_interface.index. */
        weapon_definition *weapon_def = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(objects, object_header_datum, weapon_index)->datum->definition_index);
        weapon_hud_index = weapon_def->weapon.interface_definition.hud_interface.index;
        if ( weapon_hud_index != -1 )
        {
            *flashlight_power = flashlight;
            return weapon_hud_index;
        }
        if ( !unit_get_weapon_count(player_unit_index) )
            hud_index = hud_globals->defaults.default_weapon_hud.index;
    }

done:
    *flashlight_power = flashlight;
    return hud_index;
}
