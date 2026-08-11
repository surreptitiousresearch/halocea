/* crosshairs_draw @0x836AFBB8 — draw every active crosshair overlay of a player's weapon-HUD interface
 * chain. Walks the root weapon-HUD tag plus up to 15 children (+0xC child index, as in
 * hud_update_weapon_local_player), and for each definition's crosshairs block draws the overlay items of
 * every crosshair whose bit is set in the runtime render mask (crosshair_hud_state.render_flags, computed
 * per-frame by hud_update_weapon_local_player) and whose map_type matches the current game mode
 * (bit 0 = in a real map, bit 1 = full screen, bit 2 = split screen).
 *
 * Per item: flash color vs flat color from the item's hud_color_definition; sprite frame either the raw
 * crosshair state value (types 0/1), or a frame-rate animation over the sprite sequence (other types);
 * the "flash when firing/throwing" types 8/9/18 additionally gate on the live weapon/unit state and expire
 * their state slot to -1 once the flash window lapses. Items with flags bit 0x10 (hide area outside
 * reticle) get their clip rectangle symmetrically expanded so the bitmap covers the whole viewport at the
 * current scale.
 *
 * DEVIATIONS (decompiler emitted "local variable allocation has failed", everything below is
 * disasm-resolved): (1) the DB prototype lists 4 args (player, weapon_index, hud_index, weapon_state) but
 * r6 is never read — real signature is the 3-arg form already used by the caller
 * hud_render_weapon_interface (r4 = hud tag index, r5 = weapon_state); (2) both hud_draw_bitmap call
 * sites: r9 = color, r10 = in_multiplayer (the split-screen byte), stack args 9/10 =
 * is_interface_bitmap = (bitmap_group->type == 4) and is_crosshair_bitmap = 1 — the decompiler had
 * scrambled these; (3) the __int64/__PAIR64__ pun pile in the clip-expansion block decodes to plain
 * (float)(__int16) conversions of the bitmap and viewport dimensions; (4) `firing_active` is initialized
 * from the high byte of the 16-bit game-mode mask, which is provably 0 (mask <= 7) — but it is NOT reset
 * per item, so crosshair type 14 reads whatever a preceding type-8/9/18 item left in it (shipped quirk,
 * reproduced). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/weapon_interface_state.h"
#include "headers/weapon_hud_globals_definition.h"
#include "headers/weapon_hud_interface_definition.h"
#include "headers/crosshair_hud_state.h"
#include "headers/weapon_hud_crosshairs_element.h"
#include "headers/weapon_crosshair_type.h"
#include "headers/weapon_hud_crosshair_item.h"
#include "headers/hud_scaling_flags.h"
#include "headers/hud_crosshair_flags.h"
#include "headers/hud_placement_definition.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/hud_color_definition.h"
#include "headers/real_rectangle2d.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/render_globals.h"
#include <string.h>
#include "headers/hud_corner.h"
#include "headers/scenario_type.h"
#include "headers/blam_data_globals.h"
#include "headers/bitmap_group_type.h"


extern int16_t local_player_count(void);
extern int game_time_get(void);
extern unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value);
extern int get_flash_duration(const hud_color_definition *hud_color);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern float hud_globals_get_scale(uint8_t in_multiplayer);
extern void hud_draw_bitmap(const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t in_multiplayer, uint8_t is_interface_bitmap, uint8_t is_crosshair_bitmap);

void crosshairs_draw(player_datum *player, int hud_index, weapon_interface_state *weapon_state)
{
    if ( hcex_off_hud_element == 3 || (weapon_hud_globals->script_flags & 1) == 0 || hud_index == -1 )
        return;

    int unit_index = player->unit_index;
    crosshair_hud_state *crosshair = &weapon_hud_globals->crosshair_state[player->local_player_index];
    const weapon_hud_interface_definition *root_definition =
            TAG_GET(const weapon_hud_interface_definition, hud_index);

    uint8_t splitscreen = 0;
    if ( local_player_count() > 1 && hcex_coop_local_player_index < 0 )
        splitscreen = 1;

    int16_t game_mode_mask;
    if ( splitscreen )
        game_mode_mask = (int16_t)((global_scenario->type != _scenario_type_main_menu) | 4);
    else
        game_mode_mask = (int16_t)((global_scenario->type != _scenario_type_main_menu) | 2);

    if ( unit_index == -1 )
        return;

    const unit_datum *unit = ((const unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    /* Root weapon-HUD interface definition plus its child chain (+0xC child tag index, -1 terminates) —
     * same walk as hud_update_weapon_local_player. */
    const weapon_hud_interface_definition *hud_definitions[16];
    hud_definitions[0] = root_definition;
    memset(&hud_definitions[1], 0, 0x3C);
    unsigned int render_flags = crosshair->render_flags;

    int definition_count = 1;
    do
    {
        int child_index = hud_definitions[definition_count - 1]->parent_hud.index;
        if ( child_index == -1 )
            break;
        hud_definitions[definition_count] =
                TAG_GET(const weapon_hud_interface_definition, child_index);
        definition_count = (int16_t)(definition_count + 1);
    }
    while ( definition_count < 16 );

    if ( definition_count <= 0 )
        return;

    /* Carried across items/crosshairs/definitions — only the type-8/9/18 gates assign it (see header). */
    uint8_t firing_active = 0;

    for ( int hud_slot = 0; hud_slot < definition_count; hud_slot = (int16_t)(hud_slot + 1) )
    {
        const weapon_hud_interface_definition *definition = hud_definitions[hud_slot];

        /* Default absolute placement: corner 4 (center), everything else zero. Rebuilt per definition,
         * copied into the live local per draw. */
        hud_absolute_placement_definition absolute_placement_template;
        absolute_placement_template.corner = _hud_corner_center;
        /* recovered: memset((char*)&tpl + 2, 0, 0x22) -> pad + unused[8] */
        absolute_placement_template.pad = 0;
        memset(absolute_placement_template.unused, 0, sizeof(absolute_placement_template.unused));

        if ( definition->crosshairs.count <= 0 )
            continue;

        for ( int crosshair_index = 0; crosshair_index < definition->crosshairs.count;
              crosshair_index = (int16_t)(crosshair_index + 1) )
        {
            const weapon_hud_crosshairs_element *element =
                &((const weapon_hud_crosshairs_element *)definition->crosshairs.address)[crosshair_index];

            if ( ((1 << element->crosshair_type) & render_flags) == 0 )
                continue;
            if ( ((1 << element->use_on_map_type) & game_mode_mask) == 0 )
                continue;

            int16_t state_index = element->crosshair_type;

            for ( int item_index = 0; item_index < element->crosshairs.items.count;
                  item_index = (int16_t)(item_index + 1) )
            {
                const weapon_hud_crosshair_item *item =
                    &((const weapon_hud_crosshair_item *)element->crosshairs.items.address)[item_index];
                unsigned int item_flags = item->flags;

                if ( item_flags & (1u << _hud_crosshair_runtime_invalid_bit) )                /* disabled */
                    continue;
                if ( (item_flags & (1u << _hud_crosshair_not_on_default_zoom_bit)) && crosshair->states[1].___u0.reference_data <= 0 )    /* show only when zoomed */
                    continue;
                if ( (item_flags & (1u << _hud_crosshair_only_on_default_zoom_bit)) && crosshair->states[1].___u0.reference_data != 0 )   /* don't show when zoomed */
                    continue;

                float scale_base;
                if ( splitscreen && (item->placement.multiplayer_scaling_flags & (1u << _hud_dont_scale_size_bit)) == 0 )
                    scale_base = 0.5f;
                else
                    scale_base = 1.0f;

                const bitmap_group_sequence *sequence;
                if ( item_flags & (1u << _hud_crosshair_not_a_sprite_bit) )   /* not a sprite */
                {
                    sequence = 0;
                }
                else
                {
                    const bitmap_group *group =
                            TAG_GET(const bitmap_group, element->crosshairs.bitmap.index);
                    sequence =
                        &((const bitmap_group_sequence *)group->sequences.address)[item->sequence_index];
                }

                float scale = scale_base * scale_crosshair;
                int16_t frame_index = 0;
                unsigned int color;

                switch ( element->crosshair_type )
                {
                    case _weapon_crosshair_aim:
                        if ( item_flags & (1u << _hud_crosshair_flashes_bit) )
                        {
                            frame_index = 0;
                            if ( crosshair->states[state_index].___u0.reference_data > 0 )
                            {
                                color = get_flash_color(&item->colors, 0);
                                goto draw;
                            }
                        }
                        else
                        {
                            frame_index = (int16_t)crosshair->states[state_index].___u0.reference_data;
                        }
                        goto flat_color;

                    case _weapon_crosshair_zoom_level:
                    {
                        int zoom_level = crosshair->states[state_index].___u0.reference_data;
                        if ( item_flags & (1u << _hud_crosshair_one_zoom_level_bit) )   /* one zoom level */
                        {
                            if ( !zoom_level )
                                goto next_item;
                            frame_index = 0;
                        }
                        else
                        {
                            frame_index = (int16_t)(zoom_level - ((item_flags >> _hud_crosshair_not_on_default_zoom_bit) & 1));
                        }
                        if ( (item_flags & (1u << _hud_crosshair_flashes_bit)) && crosshair->states[0].___u0.reference_data > 0 )
                        {
                            color = get_flash_color(&item->colors, 0);
                            goto draw;
                        }
                        goto flat_color;
                    }

                    case _weapon_crosshair_flash_when_firing_with_depleted_battery:
                        if ( weapon_state->age != 0.0f )
                        {
                            firing_active = 0;
                            goto flash_gate;
                        }
                        firing_active = (unit->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0;
                        goto flash_gate;

                    case _weapon_crosshair_flash_when_firing_and_no_ammo:
                        if ( weapon_state->magazines[0].rounds_loaded
                          || weapon_state->magazines[0].rounds_remaining )
                        {
                            firing_active = 0;
                            goto flash_gate;
                        }
                        firing_active = (unit->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0;
                        goto flash_gate;

                    case _weapon_crosshair_flash_when_throwing_and_no_grenade:
                    {
                        uint8_t no_grenades = 1;
                        for ( int grenade_type = 0; grenade_type < 2;
                              grenade_type = (int16_t)(grenade_type + 1) )
                        {
                            if ( no_grenades && !unit->unit.grenade_counts[grenade_type] )
                                continue;
                            no_grenades = 0;
                        }
                        if ( !no_grenades || unit->unit.grenade_throw_state )
                        {
                            firing_active = 0;
                            goto flash_gate;
                        }
                        firing_active = (unit->unit.control_flags & (1u << _unit_control_throw_grenade_bit)) != 0;
                        goto flash_gate;
                    }

                    case _weapon_crosshair_flash_when_firing_secondary_trigger_with_no_ammo:
                              /* no gate of its own; reads the stale firing_active (shipped quirk, see header) */
                    flash_gate:
                        if ( !firing_active )
                        {
                            int flash_duration = get_flash_duration(&item->colors);
                            if ( game_time_get() - crosshair->states[state_index].___u0.reference_data
                                    >= flash_duration )
                                goto expire_state;
                        }
                        if ( crosshair->states[state_index].___u0.reference_data == -1 )
                        {
                    expire_state:
                            crosshair->states[state_index].___u0.reference_data = -1;
                            goto next_item;
                        }
                        /* fall through to the frame animation */

                    default:  /* types 2-7, 10-13, 15-17 jump straight here */
                        if ( item->frame_rate > 0 )
                        {
                            /* NULL sequence here (flags bit 0x2 with a positive frame rate) would fault —
                             * shipped code has no guard. */
                            frame_index = (int16_t)((game_time_get()
                                    - crosshair->states[state_index].___u0.reference_data)
                                    / item->frame_rate / 30 % sequence->sprites.count);
                        }
                        else
                        {
                            frame_index = 0;
                        }
                        if ( (item_flags & (1u << _hud_crosshair_flashes_bit)) && crosshair->states[state_index].___u0.reference_data != -1 )
                        {
                            color = get_flash_color(&item->colors,
                                    crosshair->states[state_index].___u0.reference_data);
                            goto draw;
                        }
                    flat_color:
                        color = item->colors.color;
                    draw:
                    {
                        const bitmap_group *group =
                                TAG_GET(const bitmap_group, element->crosshairs.bitmap.index);
                        const bitmap_group_sprite *sprites = sequence
                                ? (const bitmap_group_sprite *)sequence->sprites.address
                                : 0;

                        int16_t bitmap_index;
                        if ( sequence )
                            bitmap_index = sprites[frame_index].bitmap_index;
                        else
                            bitmap_index = item->sequence_index;

                        bitmap_data *bitmap =
                                &((bitmap_data *)group->bitmaps.address)[bitmap_index];
                        if ( !_texture_cache_bitmap_get_hardware_format(bitmap, 0, 1) )
                            goto next_item;

                        uint8_t is_interface_bitmap = group->type == _bitmap_group_type_interface_bitmaps;

                        hud_absolute_placement_definition absolute_placement = absolute_placement_template;
                        hud_placement_definition placement = item->placement;
                        placement.offset.__s1.x = (int16_t)(placement.offset.__s1.x
                                + (int)(crosshairOffsetX * 640.0f / hud_globals_get_scale(0)));

                        if ( item_flags & (1u << _hud_crosshair_hide_outside_area_bit) )   /* hide area outside reticle: expand the clip to the viewport */
                        {
                            float clip_u0, clip_u1, clip_v0, clip_v1;
                            float texel_scale_u = 1.0f;
                            float texel_scale_v = 1.0f;

                            if ( sequence )
                            {
                                real_rectangle2d sprite_bounds = sprites[frame_index].bounds;
                                clip_u0 = sprite_bounds.n[0];
                                clip_u1 = sprite_bounds.n[1];
                                clip_v0 = sprite_bounds.n[2];
                                clip_v1 = sprite_bounds.n[3];
                            }
                            else
                            {
                                clip_u0 = 0.0f;
                                clip_u1 = (float)(is_interface_bitmap ? bitmap->width : 1);
                                clip_v0 = 0.0f;
                                clip_v1 = (float)(is_interface_bitmap ? bitmap->height : 1);
                                if ( !is_interface_bitmap )
                                {
                                    texel_scale_u = (float)((1.0f / (float)bitmap->width) * 1.25);
                                    texel_scale_v = (float)((1.0f / (float)bitmap->height) * 1.25);
                                }
                            }

                            float inverse_scale = 1.0f / scale;
                            float viewport_width = (float)(render.camera.viewport_bounds.__s1.x1
                                    - render.camera.viewport_bounds.__s1.x0);
                            float viewport_height = (float)(render.camera.viewport_bounds.__s1.y1
                                    - render.camera.viewport_bounds.__s1.y0);

                            float expand_u = -0.5f * (((float)bitmap->width
                                    - viewport_width * inverse_scale) * texel_scale_u);
                            float expand_v = -0.5f * (((float)bitmap->height
                                    - viewport_height * inverse_scale) * texel_scale_v);

                            real_rectangle2d clip;
                            clip.n[0] = clip_u0 - expand_u;
                            clip.n[1] = clip_u1 + expand_u;
                            clip.n[2] = clip_v0 - expand_v;
                            clip.n[3] = clip_v1 + expand_v;

                            hud_draw_bitmap(bitmap, &absolute_placement, &placement, &clip, scale, 0.0f,
                                    color, splitscreen, is_interface_bitmap, 1);
                        }
                        else
                        {
                            const real_rectangle2d *clip = sequence ? &sprites[frame_index].bounds : 0;
                            hud_draw_bitmap(bitmap, &absolute_placement, &placement, clip, scale, 0.0f,
                                    color, splitscreen, is_interface_bitmap, 1);
                        }
                        break;
                    }
                }
            next_item:;
            }
        }
    }
}
