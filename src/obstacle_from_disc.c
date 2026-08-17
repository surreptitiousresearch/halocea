/* obstacle_from_disc @0x8381CFD8 */
#include <stdint.h>
#include "headers/obstacles.h"

int16_t obstacle_from_disc(const obstacles *obstacles, int16_t disc_index)
{
    if ( disc_index == -1 )
        return -1;
    return obstacles->discs[disc_index].obstacle_index;
}
