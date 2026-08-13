/* hud_draw_multitexture_overlay_get_current_weapon_definition @0x8379EBF8 — resolve the weapon whose
 * interface state should currently be drawn for a player: their own current inventory weapon, or — when
 * they have none (e.g. driving a vehicle) — their seat's weapon on the vehicle they're riding, provided
 * that seat is flagged as weapon-capable (seat definition dword bit 0x8). Fails (0) if neither resolves.
 * Sibling of hud_update_weapon.c's identical per-player weapon-resolution logic. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_interface_state.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_seat_flags.h"


extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern void weapon_build_weapon_interface_state(int weapon_index, weapon_interface_state *state);

uint8_t hud_draw_multitexture_overlay_get_current_weapon_definition(const player_datum *player, weapon_interface_state *weapon_state)
{
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum;
    int weapon = unit_inventory_get_weapon(player->unit_index, unit_object->unit.current_weapon_index);

    if ( weapon == -1 )
    {
        int parent_unit_index = unit_object->object.parent_object_index;
        if ( parent_unit_index == -1 )
            return 0;

        int16_t seat_weapon_index = unit_object->unit.parent_seat_index;
        if ( seat_weapon_index == -1 )
            return 0;

        unit_datum *parent_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum;
        unit_definition *parent_definition = TAG_GET(unit_definition, parent_object->definition_index);
        /* seat flags bit 0x8: seat allows weapon HUD */
        if ( (((unit_seat *)parent_definition->unit.seats.address)[seat_weapon_index].flags & (1u << _unit_seat_is_gunner_bit)) == 0 )
            return 0;

        weapon = unit_inventory_get_weapon(parent_unit_index, parent_object->unit.current_weapon_index);
        if ( weapon == -1 )
            return 0;
    }

    weapon_build_weapon_interface_state(weapon, weapon_state);
    return 1;
}
