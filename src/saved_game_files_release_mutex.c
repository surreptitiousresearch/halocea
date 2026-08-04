/* saved_game_files_release_mutex @0x83727098 — release the saved-game-files general mutex. */

#include "headers/saved_game_files_globals.h"

#include "headers/mutex.h"
extern void release_mutex(const mutex *mutex_reference);

void saved_game_files_release_mutex(void)
{
    release_mutex(saved_game_files_globals.general_mutex);
}
