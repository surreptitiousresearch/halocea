/* game_time_get_paused @ 0x8369A908 */
#include <stdint.h>
#include "headers/game_time_globals.h"
uint8_t game_time_get_paused(void)   /* lbz r3,2(r11); callers clrlwi r3,24 => 8-bit return */
{
    return game_time_globals->paused;
}
