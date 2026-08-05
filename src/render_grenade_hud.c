/* render_grenade_hud @0x836AF870 — draw the local player's grenade-count HUD panel: skip if the HUD is
 * off, the current weapon disallows throwing grenades, or the unit has no grenade selection UI; skip
 * also if the unit is riding as a non-primary seat of a verified vehicle (parent object type 3, seat
 * dwords at +0x324/+0x328 both != unit_index). Otherwise resolves the current grenade type's HUD
 * interface tag, latches/clears the flash-reference time, and draws the background icon, the count
 * panel (icon + digits), and the state overlays.
 *
 * DEVIATION-CHECK, none needed: a prior session's memory note flagged a suspected double-indirection
 * bug in the `grenade_hud_interface_definition` pointer fetch (TAG_INSTANCE dereference). Re-traced
 * fresh via disasm this pass — the decompiler's assignment (`*(int*)TAG_INSTANCE(hud_tag_index)`) is
 * correct; disasm just folds the `+0x14` (cache_file_tag_instance.base_address) into the load's
 * displacement instead of into the address computation, which is mathematically identical. No bug.
 *
 * FAITHFUL QUIRK: the two flag-word computations (`background_draw_flags`/`overlay_type_flags`) and the
 * `in_multiplayer` byte reproduce literal PPC carry/sign-bit idioms confirmed instruction-for-instruction
 * against disasm (subfc/adde signed-comparison chains); kept as the decompiler expressed them rather than
 * reduced to a plain comparison, since downstream code ANDs/ORs specific bits of the raw result. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_grenade.h"
#include "headers/weapon_hud_globals_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/grenade_hud_interface_definition.h"
#include "headers/object_type.h"
#include "headers/hud_draw_flags.h"
#include "headers/grenade_overlay_on_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/hud_absolute_placement_definition.h"
#include "headers/number_hud_element_definition.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/static_hud_element_definition.h"
#include "headers/weapon_hud_overlay_definition.h"
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t weapon_prevents_grenade_throwing(int weapon_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t unit_get_current_grenade_type(int unit_index);
extern int16_t unit_get_grenade_count(int unit_index, int16_t grenade_type);
extern int game_time_get(void);
extern int16_t local_player_count(void);
extern void hud_draw_static_element(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const static_hud_element_definition *static_element, int16_t draw_flags, int flash_reference_time);
extern void hud_draw_numbers(int16_t local_player_index, const hud_absolute_placement_definition *placement, const number_hud_element_definition *numbers, int16_t value, int16_t decimal_value, int16_t draw_flags, int flash_reference_time, float override_scale);
extern void hud_draw_weapon_overlays(int16_t local_player_index, const hud_absolute_placement_definition *placement, const weapon_hud_overlay_definition *overlays, int type_flags, int reference_time, int16_t draw_flags, uint8_t in_multiplayer);

void render_grenade_hud(int16_t local_player_index, int unit_index)
{
    unit_datum *obj = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int weapon = unit_inventory_get_weapon(unit_index, obj->unit.current_weapon_index);

    if (hcex_off_hud_element == 4 || weapon_prevents_grenade_throwing(weapon) || obj->unit.current_grenade_index == 255)
        return;

    unit_datum *parent_vehicle = object_try_and_get_and_verify_type(obj->object.parent_object_index, object_mask_unit);
    if (parent_vehicle && (parent_vehicle->unit.driver_object_index == unit_index
                        || parent_vehicle->unit.gunner_object_index == unit_index))
        return;

    int16_t current_grenade_type = unit_get_current_grenade_type(unit_index);
    if (current_grenade_type == -1)
        return;

    game_globals_grenade *grenade = &((game_globals_grenade *)global_game_globals->grenades.address)[current_grenade_type];
    weapon_hud_state *hud_state = &weapon_hud_globals->hud_state[local_player_index];

    int hud_tag_index = grenade->hud_interface.index;
    if (hud_tag_index == -1)
        return;

    char selector = obj->unit.current_grenade_index;
    char selected_byte = obj->unit.grenade_counts[selector];
    grenade_hud_interface_definition *ghi = TAG_GET(grenade_hud_interface_definition, hud_tag_index);
    int16_t flash_cutoff = ghi->grenade_count_panel.flash_cutoff;

    unsigned int background_flag_bits = ((uint64_t)flash_cutoff >> 32)
        + (flash_cutoff >= (unsigned int)selected_byte)
        + ((unsigned int)selected_byte >> 31);
    int16_t background_draw_flags = selected_byte
        ? (background_flag_bits & ~(1u << _hud_draw_disabled_bit))
        : (background_flag_bits | (1u << _hud_draw_disabled_bit));
    background_draw_flags = (local_player_count() <= 1)
        ? (background_draw_flags & ~(1u << _hud_draw_in_multiplayer_bit))
        : (background_draw_flags | (1u << _hud_draw_in_multiplayer_bit));

    if (background_draw_flags & (1u << _hud_draw_flashing_bit))
    {
        if (hud_state->last_grenade_flash_time == -1)
            hud_state->last_grenade_flash_time = game_time_get();
    }
    else
    {
        hud_state->last_grenade_flash_time = -1;
    }

    if (ghi->background.interface_bitmap.index != -1)
        hud_draw_static_element(local_player_index, &ghi->absolute_placement, &ghi->background,
                                background_draw_flags, hud_state->last_grenade_flash_time);

    if (ghi->grenade_count_panel.background.interface_bitmap.index != -1)
        hud_draw_static_element(local_player_index, &ghi->absolute_placement,
                                &ghi->grenade_count_panel.background, background_draw_flags,
                                hud_state->last_grenade_flash_time);

    if (ghi->grenade_count_panel.numbers.digits)
    {
        int flash_reference_time = hud_state->last_grenade_flash_time;
        int16_t grenade_type_for_count = unit_get_current_grenade_type(unit_index);
        int16_t grenade_count = unit_get_grenade_count(unit_index, grenade_type_for_count);
        hud_draw_numbers(local_player_index, &ghi->absolute_placement,
                         &ghi->grenade_count_panel.numbers, grenade_count, -1, background_draw_flags,
                         flash_reference_time, 0.0f);
    }

    if (ghi->overlays.bitmap.index != -1)
    {
        char selector2 = obj->unit.current_grenade_index;
        char selected_byte2 = obj->unit.grenade_counts[selector2];

        /* DEVIATION: disasm shows the identical `srawi ...,0x1F` sign-replicate op as the background
         * block's first term above (same instruction pattern, both operating on flash_cutoff); the
         * decompiler displayed this occurrence as a plain `>>31` (giving 0/1) instead of the equivalent
         * `(unsigned __int64)x>>32` form (giving 0/-1) it used for the first occurrence. Corrected to
         * match the real (identical) hardware computation. */
        unsigned int overlay_flag_bits = ((uint64_t)flash_cutoff >> 32)
            + (flash_cutoff >= (unsigned int)selected_byte2)
            + ((unsigned int)selected_byte2 >> 31);
        int16_t overlay_type_flags = selected_byte2
            ? (overlay_flag_bits & ~(1u << _grenade_overlay_on_empty_bit))
            : (overlay_flag_bits | (1u << _grenade_overlay_on_empty_bit));
        overlay_type_flags = overlay_type_flags
            ? (overlay_type_flags & ~(1u << _grenade_overlay_on_default_bit))
            : (1u << _grenade_overlay_on_default_bit);
        overlay_type_flags |= 1u << _grenade_overlay_on_always_bit;

        int16_t player_count = local_player_count();
        uint8_t in_multiplayer = ((player_count >= 0) + ((unsigned int)player_count <= 1)) & 1;

        hud_draw_weapon_overlays(local_player_index, &ghi->absolute_placement, &ghi->overlays,
                                 overlay_type_flags, hud_state->last_grenade_flash_time, background_draw_flags,
                                 in_multiplayer);
    }
}
