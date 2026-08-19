/* sound_refresh_looping_not @0x83716A30 — keeps a playing looping sound alive for this mix frame by
 * stamping its channel's flip-flop marker with the current value, so the mixer doesn't reap it. Does
 * nothing if the sound manager is not initialized/active or sound is disabled from the command line.
 *
 * Mixer looping-sound record is 228 bytes; flip-flop marker byte @76. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/data_array.h"
#include "headers/looping_sound_datum.h"
#include "headers/blam_data_globals.h"


extern int looping_sound_find(int identifier);

void sound_refresh_looping_not(int identifier, uint8_t alternate)
{
    if ( !sound_manager_globals.initialized
      || !sound_manager_globals.active
      || sound_manager_globals.disable_from_commandline )
        return;

    {
        int index = looping_sound_find(identifier);
        if ( index != -1 )
            /* raw `228*index + 76` typed via DB: looping_sound_datum (228B), .flip_flop @76 */
            DATUM_GET(looping_sound_data, looping_sound_datum, index)->flip_flop =
                sound_manager_globals.flip_flop;
    }
}
