/* hud_render_unit_interface @0x836D6C60 — render the unit HUD (shield/health/motion-sensor/auxilary
 * panels + team overlay) for one local player's unit and every unit sharing its vehicle.
 *
 * Builds a stack of HUD "layers": the player's own unit, its parent vehicle (if the seat exposes a HUD),
 * and every other unit seated in that same vehicle. Each layer resolves to a unit_hud_interface_definition
 * tag which is then drawn top-of-stack downward. For the bottom-most layer (the player's own unit) the
 * per-flash timers in the persistent unit_hud_state are updated; upper (vehicle/passenger) layers draw
 * with live vitalities and no timer bookkeeping. After the panels, the motion sensor, auxilary overlays,
 * auxilary meters, and (in game-engine modes) the team/change-color icon are drawn.
 *
 * CAVEAT: reconstructed from a "local variable allocation has failed, the output may be wrong" decompile
 * with heavy 64-bit register packing. Notes on the best-effort untangling (all verified against the
 * element/tag layouts in the database):
 *  - The overcharge shield loop's segment index was emitted as an __int64 with both halves aliased to the
 *    loop counter (LODWORD/HIDWORD(v44)); it is really the scalar integer segment index used as a float.
 *  - The per-segment overcharge tint colors were written through the reused v141 hud placement struct and
 *    walked by an advancing dword pointer; they are reconstructed here as a fixed 5-entry color table
 *    (index 0 unused — segment 0 draws with the meter's own colors). v141 itself is the motion-sensor
 *    placement (corner = 2) further down and is kept separate.
 *  - hud_calculate_point's output point (decompiler local v74) was never assigned in the failed alloc; it
 *    is unified here with the point actually handed to motion_sensor_draw_screen (blip_screen_point).
 *  - hud_draw_bitmap_internal's trailing color/extra args (decompiler v115/v117 and the dropped 9th arg)
 *    were reg-alloc garbage; the team-background/icon colors are passed as the reconstructed change-color
 *    (FFA) or opaque white, and the 9th arg as 0. The bitmap selection, positions, scales and clip/bounds
 *    rects are faithful.
 *  - object payload fields are read through the DB-verified unit_datum (object/unit sub-blocks).
 */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/auxilary_overlay_use_team_color_flags.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_hud_globals.h"
#include "headers/unit_hud_interface_definition.h"
#include "headers/auxilary_overlay_definition.h"
#include "headers/auxilary_meter_definition.h"
#include "headers/meter_hud_element_definition.h"
#include "headers/static_hud_element_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/hud_color_definition.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector2d.h"
#include "headers/point2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/bitmap_data.h"
#include "headers/game_variant.h"
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"

#include <string.h>
#include "headers/game_engine_type.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_seat_flags.h"
#include "headers/hud_corner.h"
#include "headers/hud_draw_flags.h"
#include "headers/hud_panel_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

/* object-table lookup: element[index].object pointer (3-dword stride), matching the decompiler idiom. */

/* engine globals — declared, not owned here */

/* persistent, HUD-local cached team bitmap group indices */
static int s_team_icon_bitmap_index;
static int s_team_background_bitmap_index;

/* helper boundaries */
#include "headers/rasterizer_meter_parameters.h"
#include "headers/hud_color_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/static_hud_element_definition.h"
extern int16_t local_player_count(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern int game_time_get(void);
extern int16_t game_time_get_elapsed(void);
extern int unit_definition_get_active_hud_index(const unit_definition *unit_definition, uint8_t in_multiplayer);
extern int unit_definition_get_seat_active_hud_index(const unit_definition *unit_definition, int16_t seat_index, uint8_t in_multiplayer);
extern int game_engine_allow_integrated_lights(int object_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int get_flash_duration(const hud_color_definition *hud_color);

extern uint8_t game_engine_has_shield(int player_index);
extern uint8_t game_engine_has_teams(void);
extern uint8_t game_engine_running(void);
extern uint8_t game_engine_hud_draw_motion_sensor(int player_index);
extern game_variant *game_engine_get_variant(void);
extern real_rgb_color *game_engine_player_get_change_color(real_rgb_color *result, int player_index);

extern int tag_loaded(uint32_t group_tag, const char *name);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);

extern void hud_draw_static_element(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const static_hud_element_definition *static_element, int16_t draw_flags, int flash_reference_time);
extern void hud_draw_meter(int16_t local_player_index, const hud_absolute_placement_definition *placement, const meter_hud_element_definition *meter, uint8_t min_value, uint8_t max_value, int16_t draw_flags, float reference_time, float reference_value);
extern void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result);
extern void motion_sensor_draw_screen(int16_t local_player_index, uint8_t in_multiplayer, const point2d *pt);
extern void hud_draw_bitmap_internal(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const point2d *point, const real_rectangle2d *clip, const real_rectangle2d *bounds, const real_vector2d *xy_scale, float theta, unsigned int color);

static float clamp_unit_fraction(float value)
{
    if (value < 0.0f)
        return 0.0f;
    if (value > 1.0f)
        return 1.0f;
    return value;
}

static uint8_t clamp_meter_byte(int value)
{
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return (uint8_t)value;
}

extern void initialize_hud_state(unit_hud_state *hud_state);
void hud_render_unit_interface(player_datum *player)
{
    int16_t local_player_index = player->local_player_index;
    if (local_player_index != render.local_player_index || player->unit_index == -1)
        return;

    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);
    const unit_definition *unit_def =
        TAG_GET(const unit_definition, unit_object->definition_index);
    int player_index = local_player_get_player_index(local_player_index);
    unit_hud_state *hud_state = &unit_hud_globals->hud_states[local_player_index];

    /* per-layer unit indices and resolved unit_hud_interface tag indices (up to 18 layers) */
    int hud_layer_unit_indices[18];
    int hud_layer_interface_indices[20];
    hud_layer_unit_indices[0] = player->unit_index;
    memset(&hud_layer_unit_indices[1], 0, 0x44u);
    hud_layer_interface_indices[0] =
        unit_definition_get_active_hud_index(unit_def, local_player_count() > 1);
    memset(&hud_layer_interface_indices[1], 0, 0x44u);

    int hud_layer_count = 1;
    uint8_t allow_integrated_lights = game_engine_allow_integrated_lights(player->unit_index);

    if (hud_state->last_unit_index == -1)
    {
        /* DEVIATION: collapsed verbatim-inlined copy of initialize_hud_state@0x836D65E8 (zero-xref donor); host had re-fetched &unit_hud_globals->hud_states[player->local_player_index] (dup datum re-fetch — aliases the already-resolved hud_state pointer) instead of reusing hud_state, now passed directly. */
        initialize_hud_state(hud_state);
    }
    hud_state->last_unit_index = player->unit_index;

    /* if the unit is seated in a vehicle that exposes a HUD, add the vehicle and its passengers */
    int parent_object_index = unit_object->object.parent_object_index;
    if (parent_object_index != -1)
    {
        int16_t parent_seat_index = unit_object->unit.parent_seat_index;
        if (parent_seat_index != -1)
        {
            unit_datum *parent_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum);
            const unit_definition *parent_def =
                TAG_GET(const unit_definition, parent_object->definition_index);
            const unit_seat *parent_seats = (const unit_seat *)parent_def->unit.seats.address;
            int parent_active_hud = unit_definition_get_active_hud_index(parent_def, local_player_count() > 1);

            /* seat->flags bit 2 (0x4): driver seat exposes the unit HUD */
            if ((parent_seats[parent_seat_index].flags & (1u << _unit_seat_is_driver_bit)) != 0)
            {
                if (parent_active_hud != -1)
                {
                    hud_layer_count = 2;
                    hud_layer_unit_indices[1] = parent_object_index;
                    hud_layer_interface_indices[1] = parent_active_hud;
                }

                int seated_unit_index = parent_object->object.first_child_object_index;
                if (seated_unit_index != -1)
                {
                    int layer_slot = hud_layer_count;
                    do
                    {
                        if (hud_layer_count >= 18)
                            break;
                        unit_datum *seated_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seated_unit_index)->datum);
                        unit_datum *seated_unit =
                            object_try_and_get_and_verify_type(seated_unit_index, object_mask_unit);
                        if (seated_unit
                            && seated_unit->object.parent_object_index == parent_object_index
                            && seated_unit->unit.parent_seat_index != (int16_t)0xFFFF)
                        {
                            hud_layer_unit_indices[layer_slot] = seated_unit_index;
                            ++hud_layer_count;
                            hud_layer_interface_indices[layer_slot++] =
                                unit_definition_get_seat_active_hud_index(parent_def,
                                    seated_unit->unit.parent_seat_index, local_player_count() > 1);
                        }
                        seated_unit_index = seated_object->object.next_object_index;
                    }
                    while (seated_unit_index != -1);
                }
            }
        }
    }

    /* auxilary-meter active mask (bit N = auxilary type N is on) */
    int aux_active_mask = 1;
    if (!allow_integrated_lights || unit_object->unit.integrated_light_power != 1.0f)
        aux_active_mask = 0;

    /* auxilary-meter low-battery flash mask */
    int aux_flash_mask = 1;
    if ((unit_object->unit.flags & (1u << _unit_integrated_light_on_bit)) != 0
        || unit_object->unit.integrated_light_battery >= 0.2f
        || (unit_object->unit.control_flags & (1u << _unit_control_integrated_light_bit)) == 0)
    {
        aux_flash_mask = 0;
    }

    float aux_meter_values[2];
    aux_meter_values[0] = unit_object->unit.integrated_light_battery;

    if (!hud_layer_count)
        return;

    const char *team_icon_king    = "ui\\shell\\bitmaps\\team_icon_king";
    const char *team_icon_slayer  = "ui\\shell\\bitmaps\\team_icon_slayer";
    const char *team_icon_oddball = "ui\\shell\\bitmaps\\team_icon_oddball";
    const char *team_icon_ctf     = "ui\\shell\\bitmaps\\team_icon_ctf";
    const char *team_icon_race    = "ui\\shell\\bitmaps\\team_icon_race";
    const char *team_background    = "ui\\shell\\bitmaps\\team_background";

    for (;;)
    {
        int layer = --hud_layer_count;
        unit_datum *layer_object =
            object_try_and_get_and_verify_type(hud_layer_unit_indices[layer], object_mask_unit);
        int interface_index = -1;
        if (layer_object)
        {
            interface_index = hud_layer_interface_indices[layer];
            if (interface_index != -1)
            {
                const unit_hud_interface_definition *hud_def =
                    TAG_GET(const unit_hud_interface_definition, interface_index);

                /* background panel */
                if (hud_def->background.interface_bitmap.index != -1)
                {
                    int16_t draw_flags = (int16_t)((layer_object->object.damage_flags >> 1) & 2);
                    if (local_player_count() <= 1)
                        draw_flags &= ~(1u << _hud_draw_in_multiplayer_bit);
                    else
                        draw_flags |= (1u << _hud_draw_in_multiplayer_bit);
                    hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                        &hud_def->background, draw_flags, -1);
                }

                /* shield panel (with overcharge segments) */
                if (hcex_off_hud_element && game_engine_has_shield(player_index)
                    && (unit_hud_globals->script_flags & (1u << _hud_panel_shield_dont_show_bit)) == 0)
                {
                    int16_t shield_flags = 0;
                    if (layer_object->object.shield_vitality < 0.25f
                        || (unit_hud_globals->script_flags & (1u << _hud_panel_shield_blink_bit)) != 0)
                        shield_flags = 1;
                    if ((layer_object->object.damage_flags & (1u << _object_dead_bit)) != 0)
                        shield_flags |= (1u << _hud_draw_disabled_bit);
                    else
                        shield_flags &= ~(1u << _hud_draw_disabled_bit);
                    if (local_player_count() <= 1)
                        shield_flags &= ~(1u << _hud_draw_in_multiplayer_bit);
                    else
                        shield_flags |= (1u << _hud_draw_in_multiplayer_bit);

                    if (!hud_layer_count)
                    {
                        if ((shield_flags & 1) != 0)
                        {
                            if (hud_state->last_shield_flash_time == -1)
                                hud_state->last_shield_flash_time = game_time_get();
                        }
                        else
                        {
                            hud_state->last_shield_flash_time = -1;
                        }
                    }

                    if (hud_def->shield_meter.meter.meter_bitmap.index != -1)
                    {
                        game_engine_running();
                        float reference_shield = hud_layer_count
                            ? layer_object->object.shield_vitality
                            : hud_state->last_shield_vitality;

                        int16_t max_alpha = hud_def->shield_meter.meter.value_scale;
                        if (!max_alpha)
                            max_alpha = 255;

                        /* recovered: memcpy(...,0x68u) -> whole-struct copy of meter_hud_element_definition */
                        meter_hud_element_definition segment_meter = hud_def->shield_meter.meter;

                        /* per-overcharge-level tint; index 0 unused (segment 0 uses the meter's own colors) */
                        static const unsigned int overcharge_colors[5] =
                            { 0u, 16711680u, 65280u, 16776960u, 8323327u };

                        for (int segment = 0; segment <= overcharge_count; ++segment)
                        {
                            uint8_t is_first_segment = (segment == 0);
                            float shield_fraction =
                                clamp_unit_fraction(layer_object->object.shield_vitality - (float)segment);
                            float reference_fraction =
                                clamp_unit_fraction(reference_shield - (float)segment);
                            uint8_t increasing = reference_fraction > shield_fraction;
                            float peak_fraction = reference_fraction > shield_fraction
                                ? reference_fraction : shield_fraction;
                            if (shield_fraction <= 0.0f && peak_fraction <= 0.0f)
                                break;

                            segment_meter.max_color = overcharge_colors[segment];
                            segment_meter.min_color = overcharge_colors[segment];

                            float reference_time = increasing ? hud_state->fade_time : -1.0f;
                            uint8_t peak_value =
                                clamp_meter_byte((int)((float)max_alpha * peak_fraction));
                            uint8_t current_value =
                                clamp_meter_byte((int)((float)max_alpha * shield_fraction));

                            const meter_hud_element_definition *draw_meter = is_first_segment
                                ? &hud_def->shield_meter.meter : &segment_meter;
                            hud_draw_meter(local_player_index, &hud_def->absolute_placement, draw_meter,
                                current_value, peak_value, shield_flags, reference_time, shield_fraction);
                        }
                    }

                    if (hud_def->shield_meter.background.interface_bitmap.index != -1)
                        hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                            &hud_def->shield_meter.background, shield_flags, hud_state->last_shield_flash_time);
                }

                /* health panel — unless suppressed, else fall through to motion sensor / aux / team icon */
                int16_t health_flags = 0;
                int draw_health = (hcex_off_hud_element != 1 && (unit_hud_globals->script_flags & (1u << _hud_panel_health_dont_show_bit)) == 0);
                if (draw_health)
                {
                    int16_t damage_flags = layer_object->object.damage_flags;
                    if ((damage_flags & (1u << _object_shield_depleted_bit)) != 0 || (unit_hud_globals->script_flags & (1u << _hud_panel_health_blink_bit)) != 0)
                        health_flags = 1;
                    if ((damage_flags & (1u << _object_dead_bit)) != 0)
                        health_flags |= (1u << _hud_draw_disabled_bit);
                    else
                        health_flags &= ~(1u << _hud_draw_disabled_bit);
                    if (local_player_count() <= 1)
                        health_flags &= ~(1u << _hud_draw_in_multiplayer_bit);
                    else
                        health_flags |= (1u << _hud_draw_in_multiplayer_bit);

                    if (!hud_layer_count)
                    {
                        if ((health_flags & 1) != 0)
                        {
                            if (hud_state->last_health_flash_time == -1)
                                hud_state->last_health_flash_time = game_time_get();
                        }
                        else
                        {
                            hud_state->last_health_flash_time = -1;
                        }
                    }

                    if (hud_def->health_meter.meter.meter_bitmap.index != -1)
                    {
                        int16_t health_scale = hud_def->health_meter.meter.value_scale;
                        if (!health_scale)
                            health_scale = 8;

                        meter_hud_element_definition health_meter = hud_def->health_meter.meter;
                        float body = layer_object->object.body_vitality;

                        if (body < hud_def->health_meter.___u2.health_extras.max_cutoff)
                        {
                            if (body <= hud_def->health_meter.___u2.health_extras.min_cutoff)
                                health_meter.max_color = health_meter.min_color;
                            else
                                health_meter.max_color = hud_def->health_meter.___u2.health_extras.mid_color;
                            health_meter.min_color = health_meter.max_color;
                        }
                        /* if body >= max_cutoff the meter's own colors are kept unchanged */

                        uint8_t body_value =
                            clamp_meter_byte((int)((float)health_scale * body));
                        hud_draw_meter(local_player_index, &hud_def->absolute_placement, &health_meter,
                            body_value, body_value, health_flags, -1.0f, body);
                    }

                    if (hud_def->health_meter.background.interface_bitmap.index != -1)
                        hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                            &hud_def->health_meter.background, health_flags, hud_state->last_health_flash_time);
                    hud_state->last_body_vitality = layer_object->object.body_vitality;
                }

                /* motion sensor panel (bottom layer only) */
                hud_absolute_placement_definition motion_sensor_placement;
                memset(&motion_sensor_placement, 0, sizeof(motion_sensor_placement));
                if (!hcex_disable_motion_sensor && hcex_off_hud_element != 2 && !hud_layer_count
                    && (unit_hud_globals->script_flags & (1u << _hud_panel_motion_sensor_dont_show_bit)) == 0
                    && game_engine_hud_draw_motion_sensor(player_index))
                {
                    motion_sensor_placement.corner = _hud_corner_bottom_left;
                    int split_bit = local_player_count() > 1 ? (1u << _hud_draw_in_multiplayer_bit) : 0;
                    int16_t motion_flags = (unit_hud_globals->script_flags & (1u << _hud_panel_motion_sensor_blink_bit)) != 0
                        ? (int16_t)(split_bit | (1u << _hud_draw_flashing_bit)) : (int16_t)split_bit;

                    if ((motion_flags & 1) != 0)
                    {
                        if (hud_state->last_motion_sensor_flash_time == -1)
                            hud_state->last_motion_sensor_flash_time = game_time_get();
                    }
                    else
                    {
                        hud_state->last_motion_sensor_flash_time = -1;
                    }

                    if (hud_def->motion_sensor.background.interface_bitmap.index != -1)
                        hud_draw_static_element(local_player_index, &motion_sensor_placement,
                            &hud_def->motion_sensor.background, motion_flags, -1);
                    if (hud_def->motion_sensor.foreground.interface_bitmap.index != -1)
                        hud_draw_static_element(local_player_index, &motion_sensor_placement,
                            &hud_def->motion_sensor.foreground, motion_flags, -1);

                    point2d blip_screen_point;
                    uint8_t blip_in_multiplayer =
                        (local_player_count() <= 1 || hcex_coop_local_player_index >= 0) ? 0 : 1;
                    hud_calculate_point(local_player_index, &motion_sensor_placement,
                        &hud_def->blip_placement, NULL, blip_in_multiplayer, 0.0f, &blip_screen_point);

                    uint8_t sensor_in_multiplayer =
                        (local_player_count() <= 1 || hcex_coop_local_player_index >= 0) ? 0 : 1;
                    motion_sensor_draw_screen(local_player_index, sensor_in_multiplayer, &blip_screen_point);
                }

                /* auxilary team-color overlays */
                uint8_t has_teams = game_engine_has_teams() != 0;
                int overlay_split_bit = local_player_count() > 1 ? 4 : 0;
                if (hud_def->auxilary_panel.auxilary_overlays.count > 0)
                {
                    for (int16_t overlay_index = 0;
                         overlay_index < hud_def->auxilary_panel.auxilary_overlays.count;
                         overlay_index = (int16_t)(overlay_index + 1))
                    {
                        auxilary_overlay_definition *overlay =
                            (auxilary_overlay_definition *)hud_def->auxilary_panel.auxilary_overlays.address
                            + overlay_index;
                        if (((1 << (uint8_t)overlay->type) & has_teams) != 0)
                        {
                            if ((overlay->flags & (1u << _auxilary_overlay_use_team_color_bit)) != 0)
                            {
                                real_rgb_color *change_colors =
                                    layer_object->object.base_change_colors;
                                int red   = (int)(change_colors[0].n[0] * 255.0f);
                                int green = (int)(change_colors[0].n[1] * 255.0f);
                                int blue  = (int)(change_colors[0].n[2] * 255.0f);
                                overlay->static_element.colors.color =
                                    0xFF000000u | ((uint8_t)red << 16)
                                    | ((uint8_t)green << 8) | (uint8_t)blue;
                            }
                            hud_draw_static_element(local_player_index,
                                &hud_def->auxilary_panel.absolute_placement,
                                &overlay->static_element, (int16_t)overlay_split_bit, -1);
                        }
                    }
                }

                /* auxilary meters (e.g. integrated-light battery) */
                if (hud_def->auxilary_meters.count > 0)
                {
                    for (int16_t meter_index = 0;
                         meter_index < hud_def->auxilary_meters.count;
                         meter_index = (int16_t)(meter_index + 1))
                    {
                        auxilary_meter_definition *aux_meter =
                            (auxilary_meter_definition *)hud_def->auxilary_meters.address + meter_index;
                        int aux_type = aux_meter->type;

                        if ((hud_state->auxilary_active_type_flags & (1 << aux_type)) != 0
                            && ((1 << aux_type) & aux_active_mask) == 0)
                            hud_state->auxilary_flash_time[aux_type] = -1;

                        if (((1 << aux_type) & aux_active_mask) != 0)
                        {
                            int bg_bitmap_index = aux_meter->panel.background.interface_bitmap.index;
                            int meter_bitmap_index = aux_meter->panel.meter.meter_bitmap.index;

                            int split_bit = local_player_count() > 1 ? 4 : 0;
                            int16_t aux_flags = aux_meter_values[aux_type]
                                    > (double)aux_meter->panel.___u2.aux_extras.min_cutoff
                                ? (int16_t)split_bit : (int16_t)(split_bit | 1);

                            hud_state->auxilary_flash_time[aux_type] += game_time_get_elapsed();
                            hud_state->auxilary_flash_time[aux_type] %=
                                2 * get_flash_duration(&aux_meter->panel.background.colors);

                            if (bg_bitmap_index != -1)
                            {
                                int now = game_time_get();
                                hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                                    &aux_meter->panel.background, aux_flags,
                                    now - hud_state->auxilary_flash_time[aux_type]);
                            }
                            if (meter_bitmap_index != -1)
                            {
                                int scale = aux_meter->panel.meter.value_scale;
                                float value = aux_meter_values[aux_type];
                                uint8_t meter_value =
                                    clamp_meter_byte((int)((float)scale * value));
                                hud_draw_meter(local_player_index, &hud_def->absolute_placement,
                                    &aux_meter->panel.meter, meter_value, meter_value, aux_flags, -1.0f, value);
                            }
                        }
                        else if (((1 << aux_type) & aux_flash_mask) != 0
                            || (hud_state->auxilary_flash_time[aux_type] != (int16_t)0xFFFF
                                && hud_state->auxilary_flash_time[aux_type]
                                    < get_flash_duration(&aux_meter->panel.background.colors)))
                        {
                            int bg_bitmap_index = aux_meter->panel.background.interface_bitmap.index;
                            int split_bit = local_player_count() > 1 ? 4 : 0;
                            hud_state->auxilary_flash_time[aux_type] += game_time_get_elapsed();
                            if (bg_bitmap_index != -1)
                            {
                                int now = game_time_get();
                                hud_draw_static_element(local_player_index, &hud_def->absolute_placement,
                                    &aux_meter->panel.background, (int16_t)(split_bit | 1),
                                    now - hud_state->auxilary_flash_time[aux_type]);
                            }
                        }
                        else
                        {
                            hud_state->auxilary_flash_time[aux_type] = -1;
                        }
                    }
                }

                hud_state->auxilary_active_type_flags = aux_active_mask;

                /* game-engine team / change-color icon */
                if (game_engine_running())
                {
                    game_variant *variant = game_engine_get_variant();
                    if (variant)
                    {
                        unsigned int icon_color = 0xFFFFFFFFu;
                        if (!game_engine_has_teams())
                        {
                            real_rgb_color change_color_buffer[2];
                            real_rgb_color *change_color =
                                game_engine_player_get_change_color(change_color_buffer, player_index);
                            int red   = (int)(change_color->n[0] * 255.0f);
                            int green = (int)(change_color->n[1] * 255.0f);
                            int blue  = (int)(change_color->n[2] * 255.0f);
                            icon_color = 0xFF000000u | ((uint8_t)red << 16)
                                | ((uint8_t)green << 8) | (uint8_t)blue;
                        }

                        int icon_bitmap_group;
                        real_vector2d icon_scale;
                        point2d icon_point;
                        icon_scale.n[0] = 0.5f;   /* default for the cached-index edge (game_engine_index outside 1..5) */
                        icon_scale.n[1] = 0.5f;
                        icon_point.__s1.x = 0;
                        icon_point.__s1.y = 0;

                        if ((unsigned int)(variant->game_engine_index - 1) > 4)
                        {
                            icon_bitmap_group = s_team_icon_bitmap_index;
                        }
                        else if (variant->game_engine_index == game_engine_slayer)
                        {
                            icon_bitmap_group = tag_loaded(0x6269746Du /* 'bitm' */, team_icon_slayer);
                            icon_scale.n[0] = 0.5f;
                            icon_scale.n[1] = 0.5f;
                            s_team_icon_bitmap_index = icon_bitmap_group;
                            icon_point.__s1.x = 8;
                            icon_point.__s1.y = 457;
                        }
                        else if (variant->game_engine_index == game_engine_oddball)
                        {
                            icon_bitmap_group = tag_loaded(0x6269746Du /* 'bitm' */, team_icon_oddball);
                            icon_scale.n[0] = 0.55000001f;
                            icon_scale.n[1] = 0.55000001f;
                            s_team_icon_bitmap_index = icon_bitmap_group;
                            icon_point.__s1.x = 7;
                            icon_point.__s1.y = 456;
                        }
                        else if (variant->game_engine_index == game_engine_king)
                        {
                            icon_bitmap_group = tag_loaded(0x6269746Du /* 'bitm' */, team_icon_king);
                            icon_scale.n[0] = 0.52999997f;
                            icon_scale.n[1] = 0.52999997f;
                            s_team_icon_bitmap_index = icon_bitmap_group;
                            icon_point.__s1.x = 8;
                            icon_point.__s1.y = 458;
                        }
                        else
                        {
                            if (variant->game_engine_index == game_engine_ctf)
                            {
                                icon_bitmap_group = tag_loaded(0x6269746Du /* 'bitm' */, team_icon_ctf);
                                icon_scale.n[0] = 0.55000001f;
                                icon_scale.n[1] = 0.55000001f;
                                s_team_icon_bitmap_index = icon_bitmap_group;
                                icon_point.__s1.y = 456;
                            }
                            else
                            {
                                icon_bitmap_group = tag_loaded(0x6269746Du /* 'bitm' */, team_icon_race);
                                icon_scale.n[0] = 0.60000002f;
                                icon_scale.n[1] = 0.60000002f;
                                s_team_icon_bitmap_index = icon_bitmap_group;
                                icon_point.__s1.y = 453;
                            }
                            icon_point.__s1.x = 6;
                        }

                        bitmap_data *icon_bitmap =
                            bitmap_group_get_bitmap_from_sequence(icon_bitmap_group, 0, 0);
                        s_team_background_bitmap_index = tag_loaded(0x6269746Du /* 'bitm' */, team_background);
                        bitmap_data *background_bitmap =
                            bitmap_group_get_bitmap_from_sequence(s_team_background_bitmap_index, 0, 0);

                        if (icon_bitmap && background_bitmap)
                        {
                            real_rectangle2d full_clip;
                            full_clip.n[0] = 0.0f;
                            full_clip.n[1] = 1.0f;
                            full_clip.n[2] = 0.0f;
                            full_clip.n[3] = 1.0f;

                            real_vector2d background_scale;
                            background_scale.n[0] = 0.60000002f;
                            background_scale.n[1] = 0.63999999f;

                            point2d background_point;
                            background_point.__s1.x = 6;
                            background_point.__s1.y = 445;

                            real_rectangle2d bounds;
                            bounds.n[0] = 0.0f;
                            bounds.n[2] = 0.0f;
                            bounds.n[1] = 128.0f;
                            bounds.n[3] = 64.0f;
                            hud_draw_bitmap_internal(NULL, background_bitmap, &background_point, &full_clip,
                                &bounds, &background_scale, 0.0f, 0xFFFFFFFFu); /* attested 8-param: phantom trailing arg dropped */

                            bounds.n[0] = 0.0f;
                            bounds.n[2] = 0.0f;
                            bounds.n[1] = 64.0f;
                            bounds.n[3] = 64.0f;
                            hud_draw_bitmap_internal(NULL, icon_bitmap, &icon_point, &full_clip,
                                &bounds, &icon_scale, 0.0f, icon_color); /* attested 8-param: phantom trailing arg dropped */
                        }
                    }
                }
            }
        }
        if (!hud_layer_count)
            return;
    }
}
