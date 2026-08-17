/* pill_intersects_triangle2d @0x836FFAD8 — test a 2D "pill" (a line segment from base to base+height,
 * swept by width) against a triangle's three edges (p0-p1, p1-p2, p2-p0); intersects if the
 * pill crosses any edge whose outward normal faces the segment, tested via vector_intersects_pill2d.
 *
 * DEVIATION: previous source declared a phantom 7th parameter (`vertex2`) and claimed p0 was never
 * dereferenced. Disasm refutes both: r9 never appears in the body (6 args, matching the DB), p0 (r6)
 * is read at 0x836FFAF0/F4, and the first edge is p1-p0 (fsubs f0,f31,f26 @0x836FFB04). The three
 * triangle points are exactly (p0, p1, p2). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t vector_intersects_pill2d(const real_point2d *point, const real_vector2d *vector, const real_point2d *base, const real_vector2d *height, float width);

uint8_t pill_intersects_triangle2d(const real_point2d *base, const real_vector2d *height, float width, const real_point2d *p0, const real_point2d *p1, const real_point2d *p2)
{
    uint8_t result = 1;

    real_vector2d edge0;
    edge0.n[0] = p1->n[0] - p0->n[0];
    edge0.n[1] = p1->n[1] - p0->n[1];
    if ( (base->n[0] - p0->n[0]) * edge0.n[1] - edge0.n[0] * (base->n[1] - p0->n[1]) > 0.0f )
    {
        if ( vector_intersects_pill2d(p0, &edge0, base, height, width) )
            return 1;
        result = 0;
    }

    real_vector2d edge1;
    edge1.n[0] = p2->n[0] - p1->n[0];
    edge1.n[1] = p2->n[1] - p1->n[1];
    if ( (base->n[0] - p1->n[0]) * edge1.n[1] - edge1.n[0] * (base->n[1] - p1->n[1]) > 0.0f )
    {
        if ( vector_intersects_pill2d(p1, &edge1, base, height, width) )
            return 1;
        result = 0;
    }

    real_vector2d edge2;
    edge2.n[0] = p0->n[0] - p2->n[0];
    edge2.n[1] = p0->n[1] - p2->n[1];
    if ( (base->n[0] - p2->n[0]) * edge2.n[1] - edge2.n[0] * (base->n[1] - p2->n[1]) > 0.0f )
        return vector_intersects_pill2d(p2, &edge2, base, height, width) != 0;

    return result;
}
