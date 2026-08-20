/* render_motion_sensor @0x837BD7E8 — draw one local player's motion-sensor (radar) HUD: the fading trail of
 * the last 10 sensor snapshots plus any custom goal blips. For each of the 10 history slots (walked newest-
 * first from sensor_active_index) it derives a fade weight w = (10 - i) * 0.1, a fade term w*w, and a blip
 * radius pow(1 - w, 3.5) * 7 + 1, then renders every non-hidden blip in that slot's 16-blip array (blip
 * coordinates are signed bytes scaled by motion_sensor_range / 127). Blips of type 6 are always hidden;
 * additionally, when a game engine is running with a variant that has the "hide enemy blips" flag (0x40),
 * blips of type 2 or 4 are hidden. After the per-slot blips it draws the slot's active custom blips (type 5),
 * gated on their goal still being in use. The whole pass is bracketed by
 * rasterizer_hud_motion_sensor_blip_begin/_end, the latter given the sweep center and current sweep angle.
 *
 * Sibling of update_motion_sensor / render_blip. In split-screen the sensor center is offset by
 * split_sensor_ofsx/ofsy and the blip scale reduced to three quarters, not halved: scale_2 = 0.75
 * (__real_3f400000 @0x837BD8D0-D4) versus 1.0 (__real_3f800000 @0x837BD8C0/8E0).
 *
 * DEVIATIONS (Hex-Rays emitted "local variable allocation has failed"): the __PAIR64__/SBYTE puns around the
 * blip coordinates decode to plain signed-char-to-float conversions; the (active - i + 10) % 10 slot index is
 * a mulhw /10 idiom; the three render_blip float args are relative_scale = motion_sensor_scale /
 * motion_sensor_range, fade = w*w, radius = pow(1 - w, 3.5)*7 + 1 (all disasm-confirmed at
 * 0x837BD860-0x837BDA94). center_point/scale_2/blip_player_index/sweep_theta are the globals the blip
 * begin/draw/end helpers read. */

#include <stdint.h>
#include "headers/motion_sensor_globals.h"
#include "headers/hud_globals.h"
#include "headers/game_variant.h"
#include "headers/real_point2d.h"
#include "headers/point2d.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/blip_type.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


extern int16_t director_get_perspective(int16_t local_player_index);
extern void rasterizer_hud_motion_sensor_blip_begin(void);
extern void rasterizer_hud_motion_sensor_blip_end(const real_point2d *center_point, float theta);
extern void render_blip(motion_sensor_datum *sensor_datum, char blip_type, char blip_size_type, const real_point2d *point, float relative_scale, float fade, float radius);
extern uint8_t game_engine_running(void);
extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_get_goal_in_use(int16_t index);
extern double pow(double base, double exponent);
extern void tiny_point2d_get(tiny_point2d *tiny_position, real_point2d *position);

void render_motion_sensor(int16_t local_player_index, uint8_t in_multiplayer, const point2d *reference)
{
    int16_t perspective = director_get_perspective(local_player_index);
    if ( perspective == _director_perspective_neutral || perspective == _director_perspective_scripted )
        return;

    real_point2d center;
    center.n[0] = (float)reference->n[0];
    center.n[1] = (float)reference->n[1];

    float relative_scale = hud_globals->defaults.motion_sensor_scale
            / hud_globals->defaults.motion_sensor_range;

    motion_sensor_player_datum *player_sensor = &motion_sensor_globals->motion_sensors[local_player_index];

    if ( in_multiplayer )
    {
        center.n[0] = split_sensor_ofsx + (float)reference->n[0];
        center.n[1] = split_sensor_ofsy + (float)reference->n[1];
    }

    blip_player_index = local_player_index;
    scale_2 = in_multiplayer ? 0.75f : 1.0f;
    center_point = center;

    rasterizer_hud_motion_sensor_blip_begin();

    for ( int i = 0; i < 10; i = (int16_t)(i + 1) )
    {
        int slot = (motion_sensor_globals->sensor_active_index - i + 10) % 10;
        motion_sensor_datum *sensor = &player_sensor->sensor_data[slot];

        float weight = ((float)(10 - i) * 0.1f);
        float fade = (weight * weight);
        float radius = (float)((float)pow(1.0f - weight, 3.5) * 7.0 + 1.0);

        for ( int blip_index = 0; blip_index < 16; blip_index = (int16_t)(blip_index + 1) )
        {
            motion_sensor_blip *blip = &sensor->blips[blip_index];
            uint8_t hidden = blip->type == _blip_type_none;

            if ( blip->type != _blip_type_none && game_engine_running() )
            {
                game_variant *variant = game_engine_get_variant();
                if ( variant )
                {
                    uint8_t hide_this = 0;
                    if ( (variant->universal_variant.flags & (1u << _motion_tracker_no_enemies)) != 0 )
                    {
                        char blip_type = blip->type;
                        if ( blip_type == _blip_type_enemy || blip_type == _blip_type_vehicle_enemy )
                            hide_this = 1;
                    }
                    hidden = hide_this;
                }
            }

            if ( !hidden )
            {
                /* DEVIATION: collapsed inlined copy of tiny_point2d_get@0x837BC6F0 (zero-xref) to its call. */
                real_point2d blip_point;
                tiny_point2d_get(&blip->pt, &blip_point);
                render_blip(sensor, blip->type, blip->size, &blip_point, relative_scale, fade, radius);
            }
        }

        if ( sensor->custom_blips_active )
        {
            for ( int custom_index = 0; custom_index < sensor->custom_blips_active;
                  custom_index = (int16_t)(custom_index + 1) )
            {
                if ( game_engine_get_goal_in_use(sensor->custom_blip_goal_indices[custom_index]) )
                {
                    /* DEVIATION: collapsed inlined copy of tiny_point2d_get@0x837BC6F0 (zero-xref) to its call. */
                    tiny_point2d *custom = &sensor->custom_blips[custom_index];
                    real_point2d custom_point;
                    tiny_point2d_get(custom, &custom_point);
                    render_blip(sensor, _blip_type_custom, _hud_blip_type_medium, &custom_point, relative_scale, fade, radius);
                }
            }
        }
    }

    rasterizer_hud_motion_sensor_blip_end(&center_point, sweep_theta);
}
