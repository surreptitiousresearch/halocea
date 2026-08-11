/* hud_play_sound @0x837BC478 — reconcile the HUD's looping/impulse sounds against a freshly computed set
 * of active conditions (type_flags), one bit per sound entry. For each entry (56-byte stride): if its
 * condition is no longer active, stop it (looping sounds are explicitly stopped) and clear its handle/flag;
 * if active, ensure it is playing — looping sounds are started once and left running, impulse sounds are
 * (re)triggered on a rising edge (when the per-entry flag bit was not already set). sound_handles and
 * sound_flags persist the currently-playing state across calls.
 *
 * Each entry is a sound_hud_element_definition: `sound` is a tag reference whose group tag is 'lsnd'
 * for a looping sound (otherwise impulse) and whose `index` is the sound definition. */

#include <stdint.h>
#include "headers/tag_block.h"
#include "headers/sound_hud_element_definition.h"

extern void unattached_looping_sound_stop(int looping_sound_index);
extern int unattached_looping_sound_start(int definition_index, int source_object_index, float scale);
extern void sound_stop_impulse(int sound_index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void hud_play_sound(int16_t local_player_index, int type_flags, tag_block *sounds, int *sound_handles, uint16_t *sound_flags)
{
    if (sounds->count <= 0)
        return;

    for (int i = 0; i < sounds->count; i = (int16_t)(i + 1))
    {
        const sound_hud_element_definition *entry =
            &((const sound_hud_element_definition *)sounds->address)[i];
        int looping = ((unsigned int)entry->sound.group_tag == 0x6C736E64u /* 'lsnd' */);
        int definition_index = entry->sound.index;
        float scale = entry->scale;

        if ((entry->type_flags & type_flags) == 0)
        {
            /* Condition inactive: stop the sound if it is playing. */
            int handle = sound_handles[i];
            if (handle != -1)
            {
                if (looping)
                    unattached_looping_sound_stop(handle);
                sound_handles[i] = -1;
                *sound_flags &= ~(1 << i);
            }
            continue;
        }

        /* Condition active: ensure the sound is playing. */
        if (looping)
        {
            if (sound_handles[i] == -1)
                sound_handles[i] = unattached_looping_sound_start(definition_index, -1, scale);
        }
        else
        {
            int handle = sound_handles[i];
            if (handle == -1)
            {
                sound_handles[i] = unspatialized_impulse_sound_new(definition_index, scale);
            }
            else if (((1 << i) & *sound_flags) == 0)
            {
                sound_stop_impulse(handle);
                sound_handles[i] = unspatialized_impulse_sound_new(definition_index, scale);
            }
        }
        *sound_flags |= 1 << i;
    }
}
