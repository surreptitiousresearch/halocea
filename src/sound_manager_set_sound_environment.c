/* sound_manager_set_sound_environment @0x837153C8 — set the active sound-environment tag used for
 * reverb/occlusion. */

#include "headers/sound_manager_globals.h"


void sound_manager_set_sound_environment(int environment_tag)
{
    sound_manager_globals.sound_environment_tag = environment_tag;
}
