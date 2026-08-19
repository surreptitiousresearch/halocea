/* hud_update_weapon @0x836B0E50 — refresh the weapon HUD for every local player. For each player's unit it
 * resolves the weapon in the current slot (falling back to the parent unit's weapon when riding a seat
 * whose flags inherit the driver's weapon HUD), builds the weapon interface state, updates the HCEX
 * weapon HUD, and renders the weapon's HUD interface. When the unit has no weapon at all the HUD is cleared
 * to the default weapon HUD. Each player's last rendered weapon index is cached in weapon_hud_globals.
 *
 * DEVIATION: hcex_update_weapon_state's float compass_magnitude (in f1) reserves a GPR slot (PPC FPR-shadow),
 * so its trailing weapon_datum* argument is passed on the stack from r30 — the weapon object's data pointer.
 * The decompiler lost this (showed an uninitialized v13); restored from the disassembly. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_interface_state.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/weapon_datum.h"
#include "headers/hud_globals.h"
#include "headers/weapon_hud_globals.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);
extern void weapon_build_weapon_interface_state(int weapon_index, weapon_interface_state *state);
extern float hcex_calculate_compass(int object_index);
extern void hcex_update_weapon_state(int16_t local_player_index, const weapon_definition *w,
                                     const weapon_interface_state *weapon_state, const int16_t *state_flags,
                                     const int16_t *overlay_flags, const int16_t *numbers,
                                     const float *numbers_real, float compass_magnitude, weapon_datum *wpn);
extern void hud_update_weapon_local_player(int16_t local_player_index, int weapon_index, int hud_index, weapon_interface_state *weapon_state);

void hud_update_weapon(void)
{
    int16_t local_player_index = local_player_get_next(-1);

    while (local_player_index != -1)
    {
        if (local_player_get_player_index(local_player_index) == -1)
        {
            local_player_index = local_player_get_next(local_player_index);
            continue;
        }

        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum,
                                                  local_player_get_player_index(local_player_index));
        int unit_index = player->unit_index;
        if (unit_index == -1)
        {
            local_player_index = local_player_get_next(local_player_index);
            continue;
        }

        unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int weapon = unit_inventory_get_weapon(unit_index, unit_object->unit.current_weapon_index);
        char clear_to_default = 0;

        if (weapon == -1)
        {
            int parent_unit_index = unit_object->object.parent_object_index;
            int16_t parent_seat_index = (parent_unit_index != -1) ? unit_object->unit.parent_seat_index : -1;
            if (parent_unit_index != -1 && parent_seat_index != -1)
            {
                unit_datum *parent_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum;
                unit_definition *parent_definition = TAG_GET(unit_definition, parent_object->definition_index);
                unit_seat *seats = (unit_seat *)parent_definition->unit.seats.address;
                if ((seats[parent_seat_index].flags & (1u << _unit_seat_is_gunner_bit)) != 0)
                    weapon = unit_inventory_get_weapon(parent_unit_index, parent_object->unit.current_weapon_index);
                else
                    clear_to_default = 1;
            }
        }

        weapon_interface_state weapon_state;
        if (weapon != -1)
        {
            weapon_datum *weapon_object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum;
            const weapon_definition *definition = TAG_GET(const weapon_definition, weapon_object->definition_index);
            weapon_build_weapon_interface_state(weapon, &weapon_state);
            float compass_magnitude = hcex_calculate_compass(weapon);
            hcex_update_weapon_state(local_player_index, definition, &weapon_state, nullptr, nullptr, nullptr,
                                     nullptr, compass_magnitude, weapon_object);
            int hud_index = definition->weapon.interface_definition.hud_interface.index;
            if (hud_index != -1)
                hud_update_weapon_local_player(local_player_index, weapon, hud_index, &weapon_state);
        }
        else if (!clear_to_default && !unit_get_weapon_count(unit_index))
        {
            memset(&weapon_state, 0, sizeof(weapon_state));
            int default_hud_index = hud_globals->defaults.default_weapon_hud.index;
            hud_update_weapon_local_player(local_player_index, -1, default_hud_index, &weapon_state);
        }

        weapon_hud_globals->hud_state[local_player_index].last_weapon_index = weapon;

        local_player_index = local_player_get_next(local_player_index);
    }
}
