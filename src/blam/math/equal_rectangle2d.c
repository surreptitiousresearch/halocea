/* equal_rectangle2d @0x8379A328 */
#include "headers/rectangle2d.h"

int equal_rectangle2d(const rectangle2d *r1, const rectangle2d *r2)
{
    return r1->n[0] == r2->n[0] && r1->n[1] == r2->n[1]
        && r1->n[2] == r2->n[2] && r1->n[3] == r2->n[3];
}
