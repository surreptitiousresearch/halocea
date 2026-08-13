/* hud_update_weapon_local_player @0x836B0660 — recompute one local player's weapon-crosshair overlay state
 * for the current frame. Resolves the player's unit, the runtime crosshair_hud_state, and the weapon-HUD
 * interface tag definition (plus its chain of up to 15 child interfaces). The union of the children's
 * crosshair-render masks (definition +0x9C) says which of the 19 crosshair overlays exist; for each existing
 * overlay it evaluates a per-overlay predicate over the live weapon_interface_state (heat/age/magazine ammo)
 * and the unit's flags, sets/clears the overlay's render bit, and maintains the overlay's reference_data slot
 * (a timestamp latched via game_time_get on first activation for the flashing overlays, a countdown for a
 * couple of the fixed overlays).
 *
 * Reconstructed from disassembly (0x836B0660-0x836B0BD4). The decompiler collapsed the 19-entry jump table
 * (data at 0x836B07DC) into a single degenerate `case 0`, so the per-overlay dispatch below is transcribed
 * directly from the table targets. r19 (the object_try_and_get_and_verify_type result) is used purely as the
 * unit datum POINTER for the flag reads at +0x208/+0x28D/+0x31E — the decompiler mislabeled it as the loop
 * index. The `crosshair_state[player_local_index]` access folds the +0x50 field offset into the observed
 * `(index+1)*80` arithmetic. The weapon-HUD interface tag definition is typed as
 * weapon_hud_interface_definition (valid_crosshair_types_flags @0x9C, parent_hud.index @0xC for the child
 * chain, flash_cutoffs thresholds). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_hud_interface_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/weapon_hud_globals_definition.h"
#include "headers/crosshair_hud_state.h"
#include "headers/weapon_interface_state.h"
#include <string.h>
#include "headers/unit_control_flags.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/weapon_crosshair_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern float player_control_get_autoaim_level(int16_t local_player_index);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern int game_time_get(void);

void hud_update_weapon_local_player(int16_t local_player_index, int weapon_index, int hud_index,
                                    weapon_interface_state *weapon_state)
{
    player_datum *player =
        DATA_ARRAY_ELEMENT(player_data, player_datum, local_player_get_player_index(local_player_index));

    unit_datum *unit = object_try_and_get_and_verify_type(player->unit_index, object_mask_unit);
    if ( !unit )
        return;

    int16_t player_local_index = player->local_player_index;
    crosshair_hud_state *crosshair = &weapon_hud_globals->crosshair_state[player_local_index];

    /* Root weapon-HUD interface tag definition, then walk its child-interface chain (each definition's +0xC
     * holds the next child tag index, -1 terminates). render_mask = union of every interface's +0x9C mask. */
    const weapon_hud_interface_definition *hud_definitions[16];
    /* TAG_INSTANCE already lands on element[i].base_address (+0x14) — a previous revision double-added 0x14 here. */
    hud_definitions[0] = TAG_GET(const weapon_hud_interface_definition, hud_index);
    const weapon_hud_interface_definition *weapon_hud_definition = hud_definitions[0];
    memset(&hud_definitions[1], 0, 0x3C);
    unsigned int render_mask = weapon_hud_definition->valid_crosshair_types_flags;

    int definition_index = 1;
    do
    {
        int child_index = hud_definitions[definition_index - 1]->parent_hud.index;
        if ( child_index == -1 )
            break;
        const weapon_hud_interface_definition *child = TAG_GET(const weapon_hud_interface_definition, child_index);
        hud_definitions[definition_index] = child;
        render_mask |= child->valid_crosshair_types_flags;
        definition_index = (int16_t)(definition_index + 1);
    }
    while ( definition_index < 16 );

    /* Dropping the weapon clears the whole crosshair state so a re-drawn crosshair starts fresh. */
    if ( weapon_index != weapon_hud_globals->hud_state[local_player_index].last_weapon_index
         && weapon_index == -1 )
    {
        memset(crosshair, 0, sizeof(crosshair_hud_state));
    }

    unsigned int render_flags = 0;
    for ( int crosshair_index = 0; crosshair_index < NUMBER_OF_WEAPON_CROSSHAIR_TYPES; crosshair_index = (int16_t)(crosshair_index + 1) )
    {
        unsigned int bit = 1u << crosshair_index;
        if ( (bit & render_mask) == 0 )
            continue;

        int result;
        switch ( crosshair_index )
        {
            case _weapon_crosshair_aim: /* auto-aim lock */
                if ( weapon_index == -1 )
                    result = 0;
                else
                    result = (player_control_get_autoaim_level(player_local_index) == 1.0f) ? 1 : 0;
                break;

            case _weapon_crosshair_zoom_level: /* zoom level (calls the getter twice, per disasm) */
                if ( player_control_get_zoom_level(player_local_index) == -1 )
                    result = 1;
                else
                    result = player_control_get_zoom_level(player_local_index) + 2;
                break;

            case _weapon_crosshair_charge:
                result = 0;
                break;

            case _weapon_crosshair_should_reload: /* primary magazine low-ammo (in reserve) */
                if ( weapon_state->magazines[0].rounds_remaining != 0 )
                    result = (weapon_state->magazines[0].rounds_loaded
                              <= weapon_hud_definition->flash_cutoffs.loaded_ammo) ? 1 : 0;
                else
                    result = 0;
                break;

            case _weapon_crosshair_flash_heat: /* overheat */
                result = (weapon_state->heat * 100.0f
                          >= (float)weapon_hud_definition->flash_cutoffs.heat) ? 1 : 0;
                break;

            case _weapon_crosshair_flash_total_ammo: /* primary reload-ready */
                if ( weapon_state->magazines[0].rounds_remaining
                     > weapon_hud_definition->flash_cutoffs.total_ammo )
                    result = 0;
                else
                    result = (weapon_state->magazines[0].reloading == 0) ? 1 : 0;
                break;

            case _weapon_crosshair_flash_battery: /* age / battery low */
                if ( weapon_state->age >= 1.0f )
                    result = 0;
                else
                    result = ((1.0f - weapon_state->age) * 100.0f
                              <= (float)weapon_hud_definition->flash_cutoffs.age) ? 1 : 0;
                break;

            case _weapon_crosshair_reload_overheat:
                result = weapon_state->magazines[0].reloading;
                break;

            case _weapon_crosshair_flash_when_firing_and_no_ammo: /* primary empty flash (latched) */
                if ( weapon_state->magazines[0].rounds_loaded == 0
                     && weapon_state->magazines[0].rounds_remaining == 0
                     && (unit->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
                    result = 1;
                else
                    result = (crosshair->states[crosshair_index].___u0.reference_data == -1) ? 0 : 1;
                break;

            case _weapon_crosshair_flash_when_throwing_and_no_grenade: /* melee/grenade ready flash (latched) */
            {
                int both_charges_empty = 1;
                for ( int charge = 0; charge < 2; ++charge )
                {
                    if ( unit->unit.grenade_counts[charge] != 0 )
                        both_charges_empty = 0;
                }
                if ( both_charges_empty
                     && unit->unit.grenade_throw_state == _unit_grenade_throw_idle
                     && (unit->unit.control_flags & (1u << _unit_control_throw_grenade_bit)) != 0 )
                    result = 1;
                else
                    result = (crosshair->states[crosshair_index].___u0.reference_data == -1) ? 0 : 1;
                break;
            }

            case _weapon_crosshair_low_ammo_and_none_left_to_reload: /* primary loaded low-ammo */
                if ( weapon_state->magazines[0].rounds_remaining != 0 )
                    result = 0;
                else if ( weapon_state->magazines[0].rounds_loaded == 0 )
                    result = 0;
                else
                    result = (weapon_state->magazines[0].rounds_loaded
                              <= weapon_hud_definition->flash_cutoffs.loaded_ammo) ? 1 : 0;
                break;

            case _weapon_crosshair_should_reload_secondary_trigger: /* secondary magazine low-ammo (in reserve) */
                if ( weapon_state->magazines[1].rounds_remaining != 0 )
                    result = (weapon_state->magazines[1].rounds_loaded
                              <= weapon_hud_definition->flash_cutoffs.loaded_ammo) ? 1 : 0;
                else
                    result = 0;
                break;

            case _weapon_crosshair_flash_secondary_total_ammo: /* secondary reload-ready */
                if ( weapon_state->magazines[1].rounds_remaining
                     > weapon_hud_definition->flash_cutoffs.total_ammo )
                    result = 0;
                else
                    result = (weapon_state->magazines[1].reloading == 0) ? 1 : 0;
                break;

            case _weapon_crosshair_flash_secondary_reload:
                result = weapon_state->magazines[1].reloading;
                break;

            case _weapon_crosshair_flash_when_firing_secondary_trigger_with_no_ammo: /* secondary empty flash (latched) */
                if ( weapon_state->magazines[1].rounds_loaded == 0
                     && weapon_state->magazines[1].rounds_remaining == 0
                     && (unit->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
                    result = 1;
                else
                    result = (crosshair->states[crosshair_index].___u0.reference_data == -1) ? 0 : 1;
                break;

            case _weapon_crosshair_low_secondary_ammo_and_none_left_to_reload: /* secondary loaded low-ammo */
                if ( weapon_state->magazines[1].rounds_remaining != 0 )
                    result = 0;
                else if ( weapon_state->magazines[1].rounds_loaded == 0 )
                    result = 0;
                else
                    result = (weapon_state->magazines[1].rounds_loaded
                              <= weapon_hud_definition->flash_cutoffs.loaded_ammo) ? 1 : 0;
                break;

            case _weapon_crosshair_primary_trigger_ready:
                result = weapon_state->magazines[0].can_fire;
                break;

            case _weapon_crosshair_secondary_trigger_ready:
                result = weapon_state->magazines[1].can_fire;
                break;

            case _weapon_crosshair_flash_when_firing_with_depleted_battery: /* age-full flash (latched) */
                if ( weapon_state->age == 1.0f
                     && (unit->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 )
                    result = 1;
                else
                    result = (crosshair->states[crosshair_index].___u0.reference_data == -1) ? 0 : 1;
                break;

            default:
                result = 0;
                break;
        }

        result = (int16_t)result;

        if ( result > 0 || crosshair_index == _weapon_crosshair_aim )
            render_flags |= bit;
        else
            render_flags &= ~bit;

        if ( crosshair_index == _weapon_crosshair_aim )
        {
            crosshair->states[crosshair_index].___u0.reference_data = result;
        }
        else if ( crosshair_index == _weapon_crosshair_zoom_level )
        {
            crosshair->states[crosshair_index].___u0.reference_data = result - 1;
        }
        else if ( result == 0 )
        {
            crosshair->states[crosshair_index].___u0.reference_data = -1;
        }
        else if ( crosshair->states[crosshair_index].___u0.reference_data == -1 )
        {
            crosshair->states[crosshair_index].___u0.reference_data = game_time_get();
        }
    }

    crosshair->render_flags = render_flags;
}
