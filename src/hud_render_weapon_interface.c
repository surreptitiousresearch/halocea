/* hud_render_weapon_interface @0x836B0BD8 — draw the crosshairs/ammo/grenade HUD for a player's current
 * weapon. If the player's own unit has no weapon, but it's riding in a vehicle seat whose definition allows
 * weapon HUD display (seat flags bit 0x8), falls back to that seat's own weapon. If neither has a weapon and
 * the seat doesn't explicitly disallow it, draws the default empty-crosshairs HUD.
 *
 * DEVIATION: the decompiler's own inferred prototype for crosshairs_draw carries a stray 4th argument;
 * disasm at every call site here only ever sets 3 registers (player, hud_index, weapon_state), matching the
 * signature already established by prior sessions' analysis. Object/unit field offsets follow the house
 * style used throughout this codebase (object_header_data raw datum lookup, TAG_INSTANCE tag dereference);
 * unit.parent_seat_index@752 and unit.current_weapon_index@754 confirmed against _unit_datum's DB layout. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_interface_state.h"
#include "headers/weapon_hud_globals_definition.h"
#include "headers/hud_globals.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat_flags.h"
#include "headers/unit_seat.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);
extern void weapon_build_weapon_interface_state(int weapon_index, weapon_interface_state *state);
extern void crosshairs_draw(player_datum *player, int hud_index, weapon_interface_state *weapon_state);
extern void render_weapon_hud(int hud_index, int16_t local_player_index, const weapon_definition *weapon_def, const weapon_interface_state *weapon_state, const int16_t *new_state_flags, const int16_t *new_overlay_flags, const int16_t *new_numbers);
extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void render_grenade_hud(int16_t local_player_index, int unit_index);
extern void *memset(void *dest, int value, unsigned int size);

void hud_render_weapon_interface(player_datum *player)
{
    weapon_interface_state weapon_state;
    int unit_index = player->unit_index;
    int unit = (int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    /* recovered: *(__int16 *)(unit + 754) -> unit.current_weapon_index (500+254) */
    int weapon = unit_inventory_get_weapon(unit_index, ((unit_datum *)unit)->unit.current_weapon_index);
    unsigned __int8 seat_disallows_hud = 0;

    if ( weapon == -1 )
    {
        /* recovered: *(_DWORD *)(unit + 284) -> object.parent_object_index (4+280) */
        int parent_object_index = ((unit_datum *)unit)->object.parent_object_index;
        if ( parent_object_index != -1 )
        {
            /* recovered: *(__int16 *)(unit + 752) -> unit.parent_seat_index (500+252) */
            __int16 parent_seat_index = ((unit_datum *)unit)->unit.parent_seat_index;
            if ( parent_seat_index != -1 )
            {
                int parent = (int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum;
                unit_definition *parent_definition = TAG_GET(unit_definition, *(int *)parent);
                unit_seat *seat = &((unit_seat *)parent_definition->unit.seats.address)[parent_seat_index];

                if ( (seat->flags & (1u << _unit_seat_is_gunner_bit)) != 0 )
                {
                    /* recovered: *(__int16 *)(parent + 754) -> unit.current_weapon_index (500+254) */
                    weapon = unit_inventory_get_weapon(parent_object_index, ((unit_datum *)parent)->unit.current_weapon_index);
                }
                else
                {
                    seat_disallows_hud = 1;
                }
            }
        }
    }

    if ( weapon != -1 )
    {
        int weapon_object = (int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum;
        const weapon_definition *definition = TAG_GET(const weapon_definition, *(int *)weapon_object);

        weapon_build_weapon_interface_state(weapon, &weapon_state);
        /* recovered: *(int *)((char *)definition + 0x48C) -> weapon.interface_definition.hud_interface.index */
        int hud_index = definition->weapon.interface_definition.hud_interface.index;

        if ( hud_index != -1 )
        {
            crosshairs_draw(player, hud_index, &weapon_state);
            render_weapon_hud(hud_index, player->local_player_index, definition, &weapon_state, 0, 0, 0);

            if ( local_player_get_player_index(player->local_player_index) == -1 )
            {
                object_try_and_get_and_verify_type(-1, object_mask_unit);
            }
            else
            {
                int owning_player_index = local_player_get_player_index(player->local_player_index);
                player_datum *owning_player = DATA_ARRAY_ELEMENT(player_data, player_datum, owning_player_index);
                object_try_and_get_and_verify_type(owning_player->unit_index, object_mask_unit);
            }
        }
    }
    else if ( !seat_disallows_hud && !unit_get_weapon_count(unit_index) )
    {
        memset(&weapon_state, 0, sizeof(weapon_state));
        crosshairs_draw(player, hud_globals->defaults.default_weapon_hud.index, &weapon_state);
    }

    render_grenade_hud(player->local_player_index, unit_index);
    if ( player->local_player_index != -1 )
        weapon_hud_globals->hud_state[player->local_player_index].last_weapon_index = weapon;
}
