/* hs_sound_get_gain_reference @0x837F8790 — return a pointer to the gain field of a named sound tag, for the
 * HaloScript sound-gain controls. Tries a 'snd!' (sound) tag first (gain at +40), then an 'lsnd' (looping sound)
 * tag (gain at +4 of its first track, when present). Logs and returns null if neither tag is loaded. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern void console_printf(uint8_t clear, const char *format, ...);

float * hs_sound_get_gain_reference(const char *tag_name)
{
    int sound_index = tag_loaded(0x736E6421, tag_name); /* 'snd!' */
    if (sound_index != -1)
    {
        sound_definition *sound = TAG_GET(sound_definition, sound_index);
        return &sound->gain;
    }

    int looping_index = tag_loaded(0x6C736E64, tag_name); /* 'lsnd' */
    if (looping_index != -1)
    {
        looping_sound_definition *definition = TAG_GET(looping_sound_definition, looping_index);
        if (definition->tracks.count > 0)
            return &((looping_sound_track *)definition->tracks.address)->gain;
    }

    console_printf(0, "the sound '%s' does not exist", tag_name);
    return nullptr;
}
