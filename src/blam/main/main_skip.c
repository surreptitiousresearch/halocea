/* main_skip @0x83689308 — request a cutscene skip of up to 15 ticks. */

#include <stdint.h>
#include "headers/main_globals.h"

void main_skip(int16_t ticks)
{
    if ( ticks <= 15 )
    {
        main_globals.skip_ticks = ticks;
        main_globals.cutscene_skip = 1;
    }
}
