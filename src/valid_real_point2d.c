#include <stdint.h>
#include "headers/real_point2d.h"

extern int _isnan(double x);

uint8_t valid_real_point2d(const real_point2d *p)
{
    if (_isnan(p->n[0]))
        return 0;
    if (_isnan(p->n[1]))
        return 0;
    return 1;
}
