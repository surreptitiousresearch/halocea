/* main_set_game_connection_to_film_playback @0x83689510 — request that the last recorded film be played back. */

#include "headers/main_globals.h"

void main_set_game_connection_to_film_playback(void)
{
    main_globals.playback_last_recording = 1;
}
