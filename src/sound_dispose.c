/* sound_dispose @0x837150B0 — tear down the sound system: platform dispose + invalidate/dispose the sound
 * and looping-sound datum arrays, finishing with the sound cache. */

#include "headers/sound_manager_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);
extern void data_dispose(data_array *data);
extern void sound_cache_delete(void);

/* DEVIATION: the decompiler threaded the tail call's r3 out as a return value; sound_cache_delete is
 * attested void, so r3 at blr is residue and sound_dispose returns nothing. */
void sound_dispose(void)
{
    if ( sound_manager_globals.initialized )
    {
        sound_manager_globals.platform->dispose();
        data_make_invalid(sound_data);
        data_make_invalid(looping_sound_data);
        sound_manager_globals.initialized = 0;
    }

    if ( sound_data )
        data_dispose(sound_data);
    if ( looping_sound_data )
        data_dispose(looping_sound_data);

    sound_cache_delete();
}
