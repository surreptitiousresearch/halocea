/* convert_to_title_safe_pixel_bounds @0x836A3348 — inset a pixel rectangle to the title-safe area (the inner 90%,
 * i.e. a 5% margin on every side) and return the result by value.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and modeled the 8-byte struct return
 * as a hidden __return_ptr that aliases the input; the disassembly returns the packed rectangle2d in r3. Single
 * pointer parameter. All int<->float conversions are fcfid/fctiwz artifacts restored as casts. */

#include "headers/rectangle2d.h"

rectangle2d convert_to_title_safe_pixel_bounds(rectangle2d *bounds)
{
    int left   = bounds->x0;
    int top    = bounds->y0;
    int right  = bounds->x1;
    int bottom = bounds->y1;
    int width  = right - left;
    int height = bottom - top;

    rectangle2d result;
    result.x0 = (__int16)(int)((float)width * 0.05f + (float)left);
    result.y0 = (__int16)(int)((float)height * 0.05f + (float)top);
    result.x1 = (__int16)(int)((float)width * 0.95f + (float)left);
    result.y1 = (__int16)(int)((float)height * 0.95f + (float)top);
    return result;
}
