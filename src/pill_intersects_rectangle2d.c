/* pill_intersects_rectangle2d @0x836FF950 — test a 2D "pill" (a line segment from base to base+height,
 * swept by width) against an axis-aligned rectangle's four edges, testing only the edges that face outward
 * from `base` (left edge if base.x < x0, bottom if base.y < y0, right if base.x > x1, top if base.y > y1).
 *
 * DEVIATION: the DB-declared prototype has 4 parameters (base, height, width, bounds); the decompiler
 * renders `bounds` as a phantom untyped 5th parameter (`float *a5`) while leaving the real `bounds`
 * parameter unused — confirmed via disasm that `bounds` (r6, shadowed past the float `width` arg) is what
 * every `a5[...]` access actually reads. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/real_rectangle2d.h"

extern uint8_t vector_intersects_pill2d(const real_point2d *point, const real_vector2d *vector, const real_point2d *base, const real_vector2d *height, float width);

uint8_t pill_intersects_rectangle2d(const real_point2d *base, const real_vector2d *height, float width, const real_rectangle2d *bounds)
{
    unsigned __int8 result = 1;

    if ( base->n[0] < bounds->x0 )
    {
        real_point2d corner = { bounds->x0, bounds->y0 };
        real_vector2d edge = { 0.0f, bounds->y1 - bounds->y0 };
        if ( vector_intersects_pill2d(&corner, &edge, base, height, width) )
            return 1;
        result = 0;
    }

    if ( base->n[1] < bounds->y0 )
    {
        real_point2d corner = { bounds->x0, bounds->y0 };
        real_vector2d edge = { bounds->x1 - bounds->x0, 0.0f };
        if ( vector_intersects_pill2d(&corner, &edge, base, height, width) )
            return 1;
        result = 0;
    }

    if ( base->n[0] > bounds->x1 )
    {
        real_point2d corner = { bounds->x1, bounds->y0 };
        real_vector2d edge = { 0.0f, bounds->y1 - bounds->y0 };
        if ( vector_intersects_pill2d(&corner, &edge, base, height, width) )
            return 1;
        result = 0;
    }

    if ( base->n[1] > bounds->y1 )
    {
        real_point2d corner = { bounds->x0, bounds->y1 };
        real_vector2d edge = { bounds->x1 - bounds->x0, 0.0f };
        return vector_intersects_pill2d(&corner, &edge, base, height, width) != 0;
    }

    return result;
}
