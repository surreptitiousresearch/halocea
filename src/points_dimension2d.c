/* points_dimension2d @0x837FD4A8 — classify the effective dimensionality of a set of 2D points: 0 if
 * fewer than 2 usable points, 1 if every point lies on a single line, 2 as soon as a point deviates from
 * that line by at least 0.0001 (or the line construction itself fails to converge and no line ever forms).
 * The first point only seeds the reference point; the second attempts to build the line through it and the
 * reference; subsequent points are tested against that line. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_plane2d.h"

extern double __fabs(double x);
extern real_plane2d *plane2d_from_points(real_plane2d *plane, const real_point2d *point0, const real_point2d *point1);

int16_t points_dimension2d(int16_t count, const real_point2d *points)
{
    int16_t dimension = -1;  /* was int + (__int16)/(unsigned __int16) truncation casts: the binary
                              * keeps this in a halfword (extsh/lhz), i.e. a source-level short */
    real_point2d reference_point;
    real_plane2d line;
    int16_t index = 0;

    do
    {
        if ( index >= count )
            break;

        if ( dimension == -1 )
        {
            dimension = 0;
            reference_point = points[index];
        }
        else if ( dimension )
        {
            if ( dimension == 1
              && __fabs((points[index].n[1] * line.n.n[1] + points[index].n[0] * line.n.n[0]) - line.d) >= 0.0001 )
                dimension = 2;
        }
        else if ( plane2d_from_points(&line, &points[index], &reference_point) )
        {
            dimension = 1;
        }

        index++;
    }
    while ( dimension < 2 );

    return dimension;
}
