/* hud_render_damage_indicators 0x836D7E20 — draw the local player's four directional damage indicators. The
 * player's unit must still exist (a biped/vehicle); otherwise the indicators are cleared. For each of the four
 * directions (front/left/back/right) whose timer is active (in the open range 0..30), an edge-anchored screen
 * position and a fixed rotation are computed, the indicator bitmap + bounding rect are retrieved, and the sprite
 * is drawn rotated toward that edge. Positions are anchored to the window edges with per-edge offsets and the
 * global HUD nudge (hcex_damage_indicators_ofsx/ofsy), then made viewport-relative. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/hud_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/point2d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t local_player_count(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern float hud_globals_get_scale(uint8_t in_multiplayer);
extern void player_effect_get_damage_indicators(int16_t local_player_index, uint8_t *damage_indicators);
extern void player_effect_clear_damage_indicators(int16_t local_player_index);
extern void hud_retrieve_bitmap_and_bounding_rect(int bitmap_group_index, int16_t sequence_index, int16_t frame_index, const bitmap_data **bitmap, const real_rectangle2d **clip);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void hud_draw_bitmap_direct(const bitmap_data *bitmap, int16_t placement, const point2d *point, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t is_interface_bitmap);

void hud_render_damage_indicators(int16_t local_player_index)
{
    if ( local_player_index == -1 )
        return;

    int player_index = local_player_get_player_index(local_player_index);
    if ( player_index != -1 )
        player_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                                local_player_get_player_index(local_player_index))->unit_index;

    if ( !object_try_and_get_and_verify_type(player_index, object_mask_unit) )
    {
        player_effect_clear_damage_indicators(local_player_index);
        return;
    }

    hud_globals_definition *globals = hud_globals;
    int16_t player_count = local_player_count();
    float scale = (hud_globals_get_scale(((player_count >= 0) + ((unsigned int)player_count <= 1)) & 1)
                          * hcex_damage_indicators_scale);

    uint8_t indicators[4];
    player_effect_get_damage_indicators(local_player_index, indicators);

    rectangle2d *viewport = &render.camera.viewport_bounds;
    rectangle2d *window = &render.camera.window_bounds;

    for ( int16_t direction = 0; (uint16_t)direction < 4u; ++direction )
    {
        unsigned int timer = indicators[direction];
        if ( !timer || timer >= 0x1E )
            continue;

        int x;
        int y;
        float theta;
        if ( direction == 1 )  /* left edge, pointing left */
        {
            theta = 1.5707964f;
            x = window->n[1] + globals->damage_indicators.left_offset + hcex_damage_indicators_ofsx;
            y = (viewport->n[2] + viewport->n[0]) / 2;
        }
        else if ( direction == 2 )  /* bottom edge, pointing down */
        {
            theta = 0.0f;
            x = (viewport->n[3] + viewport->n[1]) / 2;
            y = window->n[2] - globals->damage_indicators.bottom_offset - hcex_damage_indicators_ofsy;
        }
        else if ( direction )  /* direction == 3: right edge, pointing right */
        {
            theta = 4.712389f;
            x = window->n[3] - globals->damage_indicators.right_offset - hcex_damage_indicators_ofsx;
            y = (viewport->n[2] + viewport->n[0]) / 2;
        }
        else  /* direction == 0: top edge, pointing up */
        {
            theta = PI;
            x = (viewport->n[3] + viewport->n[1]) / 2;
            y = window->n[0] + globals->damage_indicators.top_offset + hcex_damage_indicators_ofsy;
        }

        /* make the anchor position viewport-relative */
        float x_relative = ((float)x - (float)viewport->n[1]);
        float y_relative = ((float)y - (float)viewport->n[0]);

        int bitmap_index = globals->damage_indicators.indicator_bitmap.index;
        int16_t sequence_index = local_player_count() <= 1
                                     ? globals->damage_indicators.sequence_index
                                     : globals->damage_indicators.multiplayer_sequence_index;

        const bitmap_data *bitmap = nullptr;
        const real_rectangle2d *clip = nullptr;
        hud_retrieve_bitmap_and_bounding_rect(bitmap_index, sequence_index, 0, &bitmap, &clip);
        /* lazy hardware-format init mutates the bitmap (rasterizer_bitmap_new); drop the const view */
        if ( bitmap && _texture_cache_bitmap_get_hardware_format((bitmap_data *)bitmap, 0, 1u) )
        {
            point2d point;
            point.__s1.x = (int)x_relative;
            point.__s1.y = (int)y_relative;
            /* DEVIATION: scale/theta are float args 5/6; on this PPC ABI they shadow r7/r8, so the next int args
             * land in r9/r10. The decompiler mistook the shadow slots for the trailing color/is_interface args
             * and left them uninitialized. */
            hud_draw_bitmap_direct(bitmap, 4, &point, clip, scale, theta,
                                   globals->damage_indicators.color, 0);
        }
    }
}
