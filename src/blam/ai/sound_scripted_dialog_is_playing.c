/* sound_scripted_dialog_is_playing @0x83715388 — true while the current game time is still before the time at
 * which no scripted dialog will be playing. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern int game_time_get(void);

uint8_t sound_scripted_dialog_is_playing(void)
{
    int now = game_time_get();
    /* branchless-signed XOR idiom == signed (now < deadline); signed compare handles game-tick wrap correctly */
    return now < sound_manager_globals.game_time_when_no_scripted_dialog_will_be_playing;
}
