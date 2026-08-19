/* sound_initialize_for_new_map @0x83715068 — revalidate the sound and looping-sound pools, unless audio was
 * disabled on the command line. */

#include "headers/sound_manager_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void sound_initialize_for_new_map(void)
{
    if ( !sound_manager_globals.disable_from_commandline )
    {
        data_make_valid(sound_data);
        data_make_valid(looping_sound_data);
    }
}
