/* read_sound_preferences @ 0x837F8DD0
   Returns the default sound preferences (no per-user prefs file in this build). */

#include "headers/sound_preferences.h"

void read_sound_preferences(sound_preferences **preferences)
{
    *preferences = &default_sound_preferences;
}
