#include <stdint.h>
#include "headers/rectangle2d.h"

int16_t rectangle2d_height(const rectangle2d *rectangle)
{
    return (short)(rectangle->n[2] - rectangle->n[0]);
}
