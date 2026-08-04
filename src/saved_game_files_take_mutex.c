/* saved_game_files_take_mutex @0x83727080 — acquire the saved-game-files general mutex with a 5000ms
 * (0x1388) timeout. */

#include <stdint.h>
#include "headers/saved_game_files_globals.h"

#include "headers/mutex.h"
extern uint8_t take_mutex(const mutex *mutex_reference, unsigned int milliseconds_to_wait);

uint8_t saved_game_files_take_mutex(void)
{
    return take_mutex(saved_game_files_globals.general_mutex, 0x1388u);
}
