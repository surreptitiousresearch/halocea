/* render_blip @0x837BCCD8 — project one motion-sensor (radar) blip from world-relative space into the HUD
 * radar disc and submit it for drawing. The incoming point is rotated by the sensor's -yaw so the radar is
 * oriented to the player's facing, range-culled against the motion-sensor range, then remapped radially with a
 * pow(dist/range, 0.7) falloff so near blips sit proportionally further out. A custom blip
 * (_blip_type_custom) scales its size by a time-driven sine pulse.
 *
 * DEVIATION: Hex-Rays emitted "local variable allocation has failed" and rendered the trig, the pow falloff,
 * and the final draw arguments as a tangle of __int64/register puns (e.g. the color pointer surfaced as the
 * string "neoffire_crouching" and the pulsing flag as `sizes`). Reconstructed from disasm 0x837BCCEC-0x837BCE80:
 *   - sensor yaw is at sensor_datum->yaw (offset 0x7C); rotation is by angle -yaw.
 *   - the range is hud_globals->defaults.motion_sensor_range (offset 0x2D0 = defaults@0x2C0 + 0x10).
 *   - the draw call is rasterizer_hud_motion_sensor_blip_draw(&blip_position, fade, size, &blip_colors[type],
 *     type == 5); its float args fade/size (f1/f2) burn the r4/r5 GPR shadow slots, so the color pointer and
 *     the pulsing flag land in r6/r7.
 * DB prototype types relative_scale/fade/radius as double (PPC FP-arg widening); the code uses them as single
 * precision, so they are typed float here. */

#include <stdint.h>
#include "headers/motion_sensor_globals.h"
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/hud_globals.h"
#include "headers/blip_type.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern double cos(double x);
extern double sin(double x);
extern double pow(double base, double exponent);
extern int game_time_get(void);
extern void rasterizer_hud_motion_sensor_blip_draw(const real_point2d *blip_position, float fade, float radius, const real_rgb_color *blip_color, uint8_t custom);

void render_blip(
    motion_sensor_datum *sensor_datum,
    char blip_type,
    char blip_size_type,
    const real_point2d *point,
    float relative_scale,
    float fade,
    float radius)
{
    real_point2d blip_position = *point;

    /* rotate the point into radar space by -yaw (cos is even, so cos(-yaw) == cos(yaw)) */
    float cos_yaw = (float)cos(-sensor_datum->yaw);
    float sin_neg_yaw = (float)sin(-sensor_datum->yaw);
    float rotated_i = blip_position.n[0] * cos_yaw - sin_neg_yaw * blip_position.n[1];
    float rotated_j = blip_position.n[0] * sin_neg_yaw + blip_position.n[1] * cos_yaw;
    blip_position.n[0] = rotated_i;
    blip_position.n[1] = rotated_j;

    float distance_squared = rotated_i * rotated_i + rotated_j * rotated_j;
    float motion_sensor_range = hud_globals->defaults.motion_sensor_range;
    if ( distance_squared >= motion_sensor_range * motion_sensor_range )
        return;

    float distance = __fsqrts(distance_squared);
    if ( distance < 0.015625f )
        distance = 0.015625f;

    float inverse_distance = 1.0f / distance;
    blip_position.n[0] = rotated_i * inverse_distance;
    blip_position.n[1] = rotated_j * inverse_distance;

    float mapped_distance = motion_sensor_range * (float)pow(distance / motion_sensor_range, 0.7);
    blip_position.n[0] = mapped_distance * blip_position.n[0] * relative_scale;
    blip_position.n[1] = mapped_distance * blip_position.n[1] * relative_scale;

    float pulse_scale = 1.0f;
    if ( blip_type == _blip_type_custom )
        /* DEVIATION: 0.10471974f was one ULP off; the image holds 0.10471976f (pi/30). */
        pulse_scale = (float)((sin((float)game_time_get() * 0.10471976f) + 1.0) * 0.3333333333333333 + 1.0);

    float blip_draw_size = pulse_scale * radius + sizes[blip_size_type];
    rasterizer_hud_motion_sensor_blip_draw(
        &blip_position, fade, blip_draw_size, &blip_colors[blip_type], blip_type == _blip_type_custom);
}
