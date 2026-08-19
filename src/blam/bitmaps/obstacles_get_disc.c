/* obstacles_get_disc @0x8381CFB8 */
#include <stdint.h>
#include "headers/obstacles.h"

const disc * obstacles_get_disc(const obstacles *obstacles, int16_t disc_index)
{
    return &obstacles->discs[disc_index];
}
