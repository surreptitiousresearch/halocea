/* pill_intersects_triangle2d @0x836FFAD8 — test a 2D "pill" (a line segment from base to base+height,
 * swept by width) against a triangle's three edges (p1-p2, p2-vertex2, vertex2-p1); intersects if the
 * pill crosses any edge whose outward normal faces the segment, tested via vector_intersects_pill2d.
 *
 * DEVIATION: the DB-declared prototype only lists 6 parameters (base, height, width, p0, p1, p2), but
 * disasm shows a genuine 7th pointer argument in r8 that supplies the triangle's third vertex for the
 * final two edge tests — added here as `vertex2`, trusting disasm over the literal DB signature (same
 * precedent as apply_vector_short_difference.c). Also per disasm, `p0` (r5) is loaded into a register but
 * never dereferenced anywhere in the function body — reproduced as a genuinely unused parameter rather
 * than assumed dead code to trim. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t vector_intersects_pill2d(const real_point2d *point, const real_vector2d *vector, const real_point2d *base, const real_vector2d *height, float width);

uint8_t pill_intersects_triangle2d(const real_point2d *base, const real_vector2d *height, float width, const real_point2d *p0, const real_point2d *p1, const real_point2d *p2, const real_point2d *vertex2)
{
    (void)p0; /* loaded but never dereferenced in the shipped code */

    uint8_t result = 1;

    real_vector2d edge0;
    edge0.n[0] = p2->n[0] - p1->n[0];
    edge0.n[1] = p2->n[1] - p1->n[1];
    if ( (base->n[0] - p1->n[0]) * edge0.n[1] - edge0.n[0] * (base->n[1] - p1->n[1]) > 0.0f )
    {
        if ( vector_intersects_pill2d(p1, &edge0, base, height, width) )
            return 1;
        result = 0;
    }

    real_vector2d edge1;
    edge1.n[0] = vertex2->n[0] - p2->n[0];
    edge1.n[1] = vertex2->n[1] - p2->n[1];
    if ( (base->n[0] - p2->n[0]) * edge1.n[1] - edge1.n[0] * (base->n[1] - p2->n[1]) > 0.0f )
    {
        if ( vector_intersects_pill2d(p2, &edge1, base, height, width) )
            return 1;
        result = 0;
    }

    real_vector2d edge2;
    edge2.n[0] = p1->n[0] - vertex2->n[0];
    edge2.n[1] = p1->n[1] - vertex2->n[1];
    if ( (base->n[0] - vertex2->n[0]) * edge2.n[1] - edge2.n[0] * (base->n[1] - vertex2->n[1]) > 0.0f )
        return vector_intersects_pill2d(vertex2, &edge2, base, height, width) != 0;

    return result;
}
