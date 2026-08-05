/* render_weapon_hud @0x836AEA00 — render one weapon's HUD interface for a local player.
 *
 * Derives the per-state flash/overlay/number arrays for the weapon (from the live weapon_interface_state, or
 * from parent-supplied arrays when the interface's flash_cutoffs.flags bit0 is set), updates the per-state
 * flash timers, walks up the parent-HUD chain recursively, then draws every static / meter / number / overlay
 * element whose runtime flags and map-type mask permit it.
 *
 * CAVEAT: reconstructed from a decompile with heavy register-allocation aliasing. Notes:
 *  - The bit-flag accumulators the decompiler emitted as unrelated __int64 pairs (v29/v56/v104/v113 with
 *    LODWORD/HIDWORD/WORD1 packing) were unpacked into their real scalar sources, verified against the
 *    disassembly at every call site (hud_draw_meter/numbers reference-time args, the second-magazine overlay
 *    recompute). The low four bits of each overlay accumulator are fully determined by the conditions, so the
 *    reg-alloc "garbage base" register the decompiler showed is irrelevant and is started from 0.
 *  - overlay_flags[0]/[1] are intentionally computed twice: first for magazine 0, then overwritten for
 *    magazine 1 (faithful to the original store order).
 *  - The autoaim "no target" sentinel is the raw NaN bit pattern 0xFFC00000 (lis r11,-0x40), stored/compared
 *    as an int, so it is written and tested through an int alias rather than a float NAN literal.
 *  - weapon_definition is an opaque boundary type; the magazine-size read keeps the decompiler's
 *    weapon->weapon.magazines.address[5] offset (0x4F4) as raw pointer math with a comment.
 */

#include <stdint.h>
#include "headers/weapon_hud_interface_definition.h"
#include "headers/player_datum.h"
#include "headers/weapon_hud_globals.h"
#include "headers/weapon_hud_static_element.h"
#include "headers/weapon_hud_meter_element.h"
#include "headers/weapon_hud_number_element.h"
#include "headers/weapon_hud_overlays_element.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_interface_state.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/scenario_type.h"
#include "headers/weapon_hud_flashing_flags.h"
#include "headers/hud_draw_flags.h"
#include "headers/weapon_overlay_on_flags.h"
#include "headers/hud_element_runtime_invalid_flags.h"
#include "headers/weapon_number_flags.h"
#include "headers/blam_data_globals.h"

/* engine globals — declared, not owned here */

/* helper boundaries */
#include "headers/hud_absolute_placement_definition.h"
#include "headers/meter_hud_element_definition.h"
#include "headers/number_hud_element_definition.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/static_hud_element_definition.h"
#include "headers/weapon_hud_overlay_definition.h"
extern int16_t local_player_count(void);
extern int game_time_get(void);
extern int player_control_get_target_object_index(int16_t local_player_index);
extern float player_control_get_autoaim_level(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

extern float  __fsqrts(float x);
extern double __fabs(double x);
extern double pow(double base, double exponent);
extern double fmod(double x, double y);

extern void hud_draw_static_element(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const static_hud_element_definition *static_element, int16_t draw_flags, int flash_reference_time);
extern void hud_draw_meter(int16_t local_player_index, const hud_absolute_placement_definition *placement, const meter_hud_element_definition *meter, uint8_t min_value, uint8_t max_value, int16_t draw_flags, float reference_time, float reference_value);
extern void hud_draw_numbers(int16_t local_player_index, const hud_absolute_placement_definition *placement, const number_hud_element_definition *numbers, int16_t value, int16_t decimal_value, int16_t draw_flags, int flash_reference_time, float override_scale);
extern void hud_draw_weapon_overlays(int16_t local_player_index, const hud_absolute_placement_definition *placement, const weapon_hud_overlay_definition *overlays, int type_flags, int reference_time, int16_t draw_flags, uint8_t in_multiplayer);

void render_weapon_hud(int hud_index, int16_t local_player_index, const weapon_definition *weapon_def, const weapon_interface_state *weapon_state, const int16_t *new_state_flags, const int16_t *new_overlay_flags, const int16_t *new_numbers)
{
    weapon_hud_interface_definition *hud_def = TAG_GET(weapon_hud_interface_definition, hud_index);
    weapon_hud_state *hud_state = &weapon_hud_globals->hud_state[local_player_index];

    int16_t state_flags[8];
    int16_t overlay_flags[8];
    int16_t number_values[8];
    float   number_fractions[8];
    int     state_index;
    int     element_index;
    int16_t flags;
    int     split_screen;

    for (state_index = 0; state_index < 8; state_index++)
        state_flags[state_index] = 0;
    for (state_index = 0; state_index < 8; state_index++)
        overlay_flags[state_index] = 0;
    for (state_index = 0; state_index < 8; state_index++)
        number_values[state_index] = 0;
    for (state_index = 0; state_index < 8; state_index++)
        number_fractions[state_index] = 0.0f;

    if (hcex_off_hud_element == 5)
        return;

    if ((hud_def->flash_cutoffs.flags & (1u << _hud_use_parent_flash_parameters_bit)) != 0 && new_state_flags && new_overlay_flags && new_numbers)
    {
        /* parent-driven: use the flag arrays supplied by the caller verbatim */
        for (state_index = 0; state_index < 8; state_index++)
            state_flags[state_index] = new_state_flags[state_index];
        for (state_index = 0; state_index < 8; state_index++)
            overlay_flags[state_index] = new_overlay_flags[state_index];
        for (state_index = 0; state_index < 8; state_index++)
            number_values[state_index] = new_numbers[state_index];
    }
    else
    {
        const weapon_flash_state_definition *cutoffs = &hud_def->flash_cutoffs;
        int age_percent;

        if (local_player_count() <= 1 || hcex_coop_local_player_index >= 0)
            split_screen = 0;
        else
            split_screen = 1;

        /* state[0] — magazine 0 total ammo */
        flags = state_flags[0];
        if (weapon_state->magazines[0].rounds_remaining > cutoffs->total_ammo) flags &= ~(1u << _hud_draw_flashing_bit); else flags |= 1u << _hud_draw_flashing_bit;
        if (weapon_state->magazines[0].rounds_remaining)                       flags &= ~(1u << _hud_draw_disabled_bit); else flags |= 1u << _hud_draw_disabled_bit;
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit; else flags &= ~(1u << _hud_draw_in_multiplayer_bit);
        state_flags[0] = flags;

        /* state[1] — magazine 0 loaded ammo */
        flags = state_flags[1];
        if (weapon_state->magazines[0].rounds_loaded > cutoffs->loaded_ammo || weapon_state->magazines[0].reloading)
            flags &= ~(1u << _hud_draw_flashing_bit);
        else
            flags |= 1u << _hud_draw_flashing_bit;
        flags &= ~((1u << _hud_draw_disabled_bit) | (1u << _hud_draw_in_multiplayer_bit));
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit;
        state_flags[1] = flags;

        /* state[2] — heat */
        flags = state_flags[2];
        if ((weapon_state->heat * 100.0f) < (float)cutoffs->heat) flags &= ~(1u << _hud_draw_flashing_bit); else flags |= 1u << _hud_draw_flashing_bit;
        flags &= ~((1u << _hud_draw_disabled_bit) | (1u << _hud_draw_in_multiplayer_bit));
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit;
        state_flags[2] = flags;

        /* state[3] — age (battery/charge) */
        flags = state_flags[3];
        if (((1.0f - weapon_state->age) * 100.0f) > (float)cutoffs->age)
            flags &= ~(1u << _hud_draw_flashing_bit);
        else
            flags |= 1u << _hud_draw_flashing_bit;
        age_percent = (int)(weapon_state->age * 100.0f);
        if (age_percent < 0) age_percent = 0; else if (age_percent > 100) age_percent = 100;
        if (age_percent == 100) flags |= 1u << _hud_draw_disabled_bit; else flags &= ~(1u << _hud_draw_disabled_bit);
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit; else flags &= ~(1u << _hud_draw_in_multiplayer_bit);
        state_flags[3] = flags;

        /* state[4] — magazine 1 total ammo */
        flags = state_flags[4];
        if (weapon_state->magazines[1].rounds_remaining > cutoffs->total_ammo) flags &= ~(1u << _hud_draw_flashing_bit); else flags |= 1u << _hud_draw_flashing_bit;
        if (weapon_state->magazines[1].rounds_remaining)                       flags &= ~(1u << _hud_draw_disabled_bit); else flags |= 1u << _hud_draw_disabled_bit;
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit; else flags &= ~(1u << _hud_draw_in_multiplayer_bit);
        state_flags[4] = flags;

        /* state[5] — magazine 1 loaded ammo */
        flags = state_flags[5];
        if (weapon_state->magazines[1].rounds_loaded > cutoffs->loaded_ammo || weapon_state->magazines[1].reloading)
            flags &= ~(1u << _hud_draw_flashing_bit);
        else
            flags |= 1u << _hud_draw_flashing_bit;
        flags &= ~((1u << _hud_draw_disabled_bit) | (1u << _hud_draw_in_multiplayer_bit));
        if (split_screen) flags |= 1u << _hud_draw_in_multiplayer_bit;
        state_flags[5] = flags;

        /* update the per-state flash timers from the low bit of each state flag */
        for (state_index = 0; state_index < 8; state_index++)
        {
            if (state_flags[state_index] & 1)
            {
                if (hud_state->last_weapon_flash_time[state_index] == -1)
                    hud_state->last_weapon_flash_time[state_index] = game_time_get();
            }
            else
            {
                hud_state->last_weapon_flash_time[state_index] = -1;
            }
        }

        /* overlay[0] — magazine 0 total ammo */
        flags = overlay_flags[0];
        if (weapon_state->magazines[0].rounds_remaining > cutoffs->total_ammo || weapon_state->magazines[0].reloading)
            flags &= ~(1u << _weapon_overlay_on_flashing_bit);
        else
            flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->magazines[0].reloading) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        if (weapon_state->magazines[0].rounds_remaining) flags &= ~(1u << _weapon_overlay_on_empty_bit); else flags |= 1u << _weapon_overlay_on_empty_bit;
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[0] = flags | (1u << _weapon_overlay_on_always_bit);

        /* overlay[1] — magazine 0 loaded ammo */
        flags = overlay_flags[1];
        if (weapon_state->magazines[0].rounds_loaded > cutoffs->loaded_ammo) flags &= ~(1u << _weapon_overlay_on_flashing_bit); else flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->magazines[0].reloading) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        if (weapon_state->magazines[0].rounds_loaded) flags &= ~(1u << _weapon_overlay_on_empty_bit); else flags |= 1u << _weapon_overlay_on_empty_bit;
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[1] = flags | (1u << _weapon_overlay_on_always_bit);

        /* overlay[2] — heat */
        flags = overlay_flags[2];
        if ((weapon_state->heat * 100.0f) < (float)cutoffs->heat) flags &= ~(1u << _weapon_overlay_on_flashing_bit); else flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->overheated) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        age_percent = (int)(weapon_state->age * 100.0f);
        if (age_percent < 0) age_percent = 0; else if (age_percent > 100) age_percent = 100;
        if (age_percent == 100) flags |= 1u << _weapon_overlay_on_empty_bit; else flags &= ~(1u << _weapon_overlay_on_empty_bit);
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[2] = flags | (1u << _weapon_overlay_on_always_bit);

        /* overlay[3] — age */
        flags = overlay_flags[3];
        if (((1.0f - weapon_state->age) * 100.0f) > (float)cutoffs->age)
            flags &= ~(1u << _weapon_overlay_on_flashing_bit);
        else
            flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->overheated) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        if (age_percent == 100) flags |= 1u << _weapon_overlay_on_empty_bit; else flags &= ~(1u << _weapon_overlay_on_empty_bit);
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[3] = flags | (1u << _weapon_overlay_on_always_bit);

        /* overlay[0] recomputed for magazine 1 (overwrites the magazine-0 value above) */
        flags = 0;
        if (weapon_state->magazines[1].rounds_remaining > cutoffs->total_ammo || weapon_state->magazines[1].reloading)
            flags &= ~(1u << _weapon_overlay_on_flashing_bit);
        else
            flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->magazines[1].reloading) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        if (weapon_state->magazines[1].rounds_remaining) flags &= ~(1u << _weapon_overlay_on_empty_bit); else flags |= 1u << _weapon_overlay_on_empty_bit;
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[0] = flags | (1u << _weapon_overlay_on_always_bit);

        /* overlay[1] recomputed for magazine 1 (overwrites the magazine-0 value above) */
        flags = 0;
        if (weapon_state->magazines[1].rounds_loaded > cutoffs->loaded_ammo) flags &= ~(1u << _weapon_overlay_on_flashing_bit); else flags |= 1u << _weapon_overlay_on_flashing_bit;
        if (weapon_state->magazines[1].reloading) flags |= 1u << _weapon_overlay_on_reload_bit; else flags &= ~(1u << _weapon_overlay_on_reload_bit);
        if (weapon_state->magazines[1].rounds_loaded) flags &= ~(1u << _weapon_overlay_on_empty_bit); else flags |= 1u << _weapon_overlay_on_empty_bit;
        if (flags) flags &= ~(1u << _weapon_overlay_on_default_bit); else flags = 1u << _weapon_overlay_on_default_bit;
        overlay_flags[1] = flags | (1u << _weapon_overlay_on_always_bit);

        /* number readouts */
        number_values[1] = weapon_state->magazines[0].rounds_loaded;
        number_values[0] = weapon_state->magazines[0].rounds_remaining;
        number_values[4] = weapon_state->magazines[1].rounds_remaining;
        number_values[5] = weapon_state->magazines[1].rounds_loaded;
        number_values[3] = (int)((1.0f - weapon_state->age) * 100.0f);
        number_values[2] = (int)(weapon_state->heat * 255.0f);

        /* autoaim target distance readouts (feet), or the "no target" NaN sentinel */
        {
            int target_object_index = player_control_get_target_object_index(local_player_index);
            if (player_control_get_autoaim_level(local_player_index) != 1.0f || target_object_index == -1)
            {
                *(unsigned int *)&number_fractions[6] = 0xFFC00000u; /* NaN sentinel: no locked target */
                *(unsigned int *)&number_fractions[7] = 0xFFC00000u;
            }
            else
            {
                int16_t player_index = local_player_get_player_index(local_player_index);
                int unit_index;
                real_point3d camera_position;
                real_point3d target_position;

                if (player_index == -1)
                    unit_index = -1;
                else
                    unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;

                unit_get_camera_position(unit_index, &camera_position);
                object_get_origin(target_object_index, &target_position);

                number_fractions[7] = (target_position.n[2] - camera_position.n[2]) * 3.0480001f;
                number_fractions[6] = __fsqrts(
                    (((camera_position.n[1] - target_position.n[1]) * (camera_position.n[1] - target_position.n[1]))
                          + (((camera_position.n[0] - target_position.n[0]) * (camera_position.n[0] - target_position.n[0]))
                                  + ((camera_position.n[2] - target_position.n[2]) * (camera_position.n[2] - target_position.n[2])))))
                    * 3.0480001f;
            }
        }
    }

    /* recurse up the parent-HUD chain, forwarding the arrays we just built */
    if (hud_def->parent_hud.index != -1)
        render_weapon_hud(hud_def->parent_hud.index, local_player_index, weapon_def, weapon_state,
            state_flags, overlay_flags, number_values);

    /* map-type mask: bit0 = in gameplay (scenario is not the main menu), bit1 = single view, bit2 = split view */
    {
        scenario *current_scenario = global_scenario;
        int16_t in_gameplay;
        int map_type_flags;

        if (local_player_count() <= 1 || hcex_coop_local_player_index >= 0)
            split_screen = 0;
        else
            split_screen = 1;

        in_gameplay = (current_scenario->type != _scenario_type_main_menu) ? 1 : 0;
        if (split_screen)
            map_type_flags = (int16_t)(in_gameplay | 4);
        else
            map_type_flags = (int16_t)(in_gameplay | 2);

        /* static elements */
        if (hud_def->statics.count > 0)
        {
            for (element_index = 0; element_index < hud_def->statics.count; element_index = (int16_t)(element_index + 1))
            {
                weapon_hud_static_element *element = (weapon_hud_static_element *)hud_def->statics.address + element_index;

                /* runtime fixup: a bad tag stored the scale as 0x3F80 (top half of 1.0f) — patch to a full 1.0f.
                 * flash_period/flash_length are floats but the fixup manipulates their raw int bit patterns. */
                /* recovered: *((int *)element + 24) -> static_element.colors.flash_period (offset 0x60) */
                if (*(int *)&element->static_element.colors.flash_period == 16256) *(int *)&element->static_element.colors.flash_period = 1065353216;
                /* recovered: *((int *)element + 27) -> static_element.colors.flash_length (offset 0x6C) */
                if (*(int *)&element->static_element.colors.flash_length == 16256) *(int *)&element->static_element.colors.flash_length = 1065353216;

                if ((element->header.runtime_flags & (1u << _hud_element_runtime_invalid_bit)) == 0
                    && ((1 << element->header.use_on_map_type) & map_type_flags) != 0)
                {
                    int16_t state = element->header.state_type;
                    hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                        &element->static_element, state_flags[state], hud_state->last_weapon_flash_time[state]);
                }
            }
        }

        /* meter elements */
        if (hud_def->meters.count > 0)
        {
            for (element_index = 0; element_index < hud_def->meters.count; element_index = (int16_t)(element_index + 1))
            {
                weapon_hud_meter_element *element = (weapon_hud_meter_element *)hud_def->meters.address + element_index;

                if ((element->header.runtime_flags & (1u << _hud_element_runtime_invalid_bit)) == 0
                    && ((1 << element->header.use_on_map_type) & map_type_flags) != 0)
                {
                    int16_t state = element->header.state_type;
                    uint8_t value = (uint8_t)number_values[state];
                    hud_draw_meter(local_player_index, &hud_def->absolute_placement, &element->meter_element,
                        value, value, state_flags[state],
                        (float)hud_state->last_weapon_flash_time[state], 0.0f);
                }
            }
        }

        /* number elements */
        if (hud_def->numbers.count > 0)
        {
            for (element_index = 0; element_index < hud_def->numbers.count; element_index = (int16_t)(element_index + 1))
            {
                weapon_hud_number_element *element = (weapon_hud_number_element *)hud_def->numbers.address + element_index;
                int16_t state;
                int16_t magazine_size;
                int16_t value;
                int16_t decimal_value;

                if ((element->header.runtime_flags & (1u << _hud_element_runtime_invalid_bit)) != 0
                    || ((1 << element->header.use_on_map_type) & map_type_flags) == 0)
                    continue;

                magazine_size = 1;
                if (element->weapon_flags & (1u << _weapon_number_divide_by_clip_size_bit))
                    /* recovered: *(void**)((char*)weapon_def+0x4F4) -> weapon.magazines.address; [5] = +0xA rounds per magazine (magazine block is a separate ledger) */
                    magazine_size = ((const int16_t *)weapon_def->weapon.magazines.address)[5];

                state = element->header.state_type;
                if (element->number_element.fractional_digits)
                {
                    float scale;
                    if (*(unsigned int *)&number_fractions[state] == 0xFFC00000u) /* NaN sentinel: skip */
                        continue;
                    scale = (float)pow(10.0, 4.0);
                    decimal_value = (int16_t)(float)fmod(__fabs((number_fractions[state] * scale)), (double)scale);
                    value = (int16_t)(number_fractions[state] / (float)magazine_size);
                }
                else
                {
                    value = number_values[state] / magazine_size;
                    decimal_value = -1;
                }

                hud_draw_numbers(local_player_index, &hud_def->absolute_placement, &element->number_element,
                    value, decimal_value, state_flags[state], hud_state->last_weapon_flash_time[state], 0.0f);
            }
        }

        /* overlay elements */
        if (hud_def->overlays.count > 0)
        {
            for (element_index = 0; element_index < hud_def->overlays.count; element_index = (int16_t)(element_index + 1))
            {
                weapon_hud_overlays_element *element = (weapon_hud_overlays_element *)hud_def->overlays.address + element_index;

                if ((element->runtime_flags & (1u << _hud_element_runtime_invalid_bit)) == 0
                    && ((1 << element->use_on_map_type) & map_type_flags) != 0)
                {
                    int16_t state = element->state_type;
                    hud_draw_weapon_overlays(local_player_index, &hud_def->absolute_placement, &element->overlays,
                        overlay_flags[state], hud_state->last_weapon_flash_time[state],
                        state_flags[state], (uint8_t)split_screen);
                }
            }
        }
    }
}
