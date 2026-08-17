/* cinematic_force_title @0x83691250 */
#include <stdint.h>
#include "headers/cinematic_globals.h"

void cinematic_force_title(int16_t index)
{
    cinematic_globals->active_titles[0].title_index = index;
    cinematic_globals->active_titles[0].title_timer = 0;
}
