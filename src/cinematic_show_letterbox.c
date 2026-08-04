/* cinematic_show_letterbox @0x83690FF0 — show or hide the cinematic letterbox bars, recording the game time
 * the bars were last shown (used to animate them in). */

#include <stdint.h>
#include "headers/cinematic_globals.h"

extern int game_time_get(void);

void cinematic_show_letterbox(uint8_t show)
{
    cinematic_globals->letter_box = show;
    if ( show )
        cinematic_globals->letter_box_last_game_time = game_time_get();
}
