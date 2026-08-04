/* apply_dead_zones @0x836FB69C — apply per-axis and radial dead zones to a raw analog stick reading.
 * The stick components arrive as signed 16-bit values in roughly [-32767, 32767]. Each axis first
 * gets a linear dead zone at +/-4095, with the live range rescaled back out to full scale
 * (32767/(32767-4095) ~= 1.1428). The dead-zoned axes are then converted to polar form: a radial
 * dead zone (again at 4095, rescaled) is applied to the magnitude while the angle is preserved, and
 * the result is written back as cartesian x = cos*radius, y = sin*radius. A magnitude at or below
 * the radial dead zone zeroes the stick.
 *
 * DEVIATION: the decompiler invented phantom FPR-shadow args (a2..a6) and a bogus string store; the
 * DB prototype is the ground truth `void apply_dead_zones(point2d *stick)`. Reconstructed from disasm. */

#include <stdint.h>
#include "headers/point2d.h"
extern float __fsqrts(float);

extern double atan2(double y, double x);
extern double sin(double x);
extern double cos(double x);
extern void set_point2d(point2d *point, int16_t x, int16_t y);

/* axis dead zone: clamp |v| to >4095, then rescale the live band back out to full scale */
static float axis_dead_zone(float v)
{
    if (v > 4095.0f)
        return (v - 4095.0f) * 1.1428223f;
    if (v < -4095.0f)
        return (v + 4095.0f) * 1.1428173f;
    return 0.0f;
}

void apply_dead_zones(point2d *stick)
{
    stick->x = (short)(int)axis_dead_zone((float)stick->x);
    stick->y = (short)(int)axis_dead_zone((float)stick->y);

    float x = (float)stick->x;
    float y = (float)stick->y;
    float angle = (float)atan2(y, x);
    float magnitude = __fsqrts(x * x + y * y);

    if (magnitude < -32768.0f) /* dead: magnitude is non-negative; preserved from disasm */
    {
        set_point2d(stick, 0, 0);
        return;
    }

    float clamped;
    if (magnitude > 32767.0f)
        clamped = 32767.0f;
    else if (magnitude <= 4095.0f)
    {
        set_point2d(stick, 0, 0);
        return;
    }
    else
        clamped = magnitude;

    float radius = (clamped - 4095.0f) * 1.1428223f;
    set_point2d(stick, (__int16)(int)((float)cos(angle) * radius),
                       (__int16)(int)((float)sin(angle) * radius));
}
