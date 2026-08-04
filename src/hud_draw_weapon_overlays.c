/* hud_draw_weapon_overlays @0x837A05D0 — draw every state-matching overlay frame of a weapon/grenade
 * HUD overlay definition: skips items disabled outright (flags bit 1) or whose `type` mask doesn't
 * intersect the caller's `type_flags`; picks the item's flat color or a flash-animated color (flags bit
 * 0, gated on `draw_flags` bit 0); picks a looping animation frame from the bitmap's own sequence table
 * when the item is both flashing and has a positive frame_rate; then resolves and draws the bitmap.
 *
 * DEVIATION: `hud_draw_bitmap_with_meter`'s DB-resolved prototype has 11 parameters, but only 10
 * distinct stack/register writes are made at this call site — the `is_interface_bitmap` stack slot
 * (between `in_multiplayer` and the explicit `is_crosshair_bitmap = 0`) is never written here, so its
 * value is whatever was already on the stack; reproduced faithfully as an uninitialized local rather
 * than guessing a value. `local_player_index` is accepted but never read in this function body. */

#include <stdint.h>
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_weapon_overlay_flags.h"
#include "headers/weapon_hud_overlay_definition.h"
#include "headers/weapon_hud_overlay_item.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

typedef struct rasterizer_meter_parameters rasterizer_meter_parameters;


#include "headers/hud_placement_definition.h"
extern int game_time_get(void);
extern unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, unsigned __int8 block, unsigned __int8 load);
extern void hud_draw_bitmap_with_meter(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color32, uint8_t in_multiplayer, uint8_t is_interface_bitmap, uint8_t is_crosshair_bitmap);

void hud_draw_weapon_overlays(int16_t local_player_index, const hud_absolute_placement_definition *placement, const weapon_hud_overlay_definition *overlays, int type_flags, int reference_time, int16_t draw_flags, uint8_t in_multiplayer)
{
    if (overlays->items.count <= 0)
        return;

    weapon_hud_overlay_item *items = (weapon_hud_overlay_item *)overlays->items.address;

    for (int i = 0; i < overlays->items.count; ++i)
    {
        weapon_hud_overlay_item *item = &items[i];

        if ((item->flags & (1u << _hud_overlay_runtime_invalid_bit)) || !(item->type & type_flags))
            continue;

        unsigned int color;
        if ((item->flags & (1u << _hud_overlay_flashes_bit)) && (draw_flags & 1))
            color = get_flash_color(&item->colors, reference_time);
        else
            color = item->colors.color;

        __int16 frame_index;
        if ((item->flags & (1u << _hud_overlay_flashes_bit)) && (draw_flags & 1) && item->frame_rate > 0)
        {
            bitmap_group *bitmap_tag_def = TAG_GET(bitmap_group, overlays->bitmap.index);
            bitmap_group_sequence *sequence_entry =
                &((bitmap_group_sequence *)bitmap_tag_def->sequences.address)[item->sequence_index];
            int frame_count = sequence_entry->sprites.count;
            int elapsed = game_time_get() - reference_time;
            frame_index = (__int16)(elapsed / item->frame_rate / 30 % frame_count);
        }
        else
        {
            frame_index = 0;
        }

        const bitmap_data *bitmap = 0;
        const real_rectangle2d *clip = 0;
        hud_retrieve_bitmap_and_bounding_rect(overlays->bitmap.index, item->sequence_index, frame_index, &bitmap, &clip);

        if (bitmap && _texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1u))
        {
            unsigned __int8 is_interface_bitmap; /* FAITHFUL QUIRK: uninitialized at this call site */
            /* recovered: (const hud_placement_definition *)item -> &item->placement (placement is at offset 0) */
            hud_draw_bitmap_with_meter(0, bitmap, placement, &item->placement, clip,
                                       hcex_hud_globals_scale, 0.0f, color, in_multiplayer, is_interface_bitmap, 0);
        }
    }
}
