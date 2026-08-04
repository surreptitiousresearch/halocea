/* obstacles_new @0x8382F994 — resets an obstacles working set to empty. */

#include "headers/obstacles.h"

void obstacles_new(obstacles *obstacles)
{
    obstacles->obstacle_count = 0;
    obstacles->disc_count = 0;
    obstacles->disc_optional_count = 0;
}
