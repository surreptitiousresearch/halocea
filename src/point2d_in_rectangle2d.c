/* point2d_in_rectangle2d @0x8379A258 */
#include "headers/rectangle2d.h"
#include "headers/point2d.h"

BOOL point2d_in_rectangle2d(const rectangle2d *rectangle, const point2d *point)
{
    if ( point->x >= rectangle->x0 && point->x < rectangle->x1 )
        return point->y >= rectangle->y0 && point->y < rectangle->y1;
    return 0;
}
