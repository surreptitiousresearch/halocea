/* hud_play_sound @0x837BC49C — reconcile the HUD's looping/impulse sounds against a freshly computed set
 * of active conditions (type_flags), one bit per sound entry. For each entry (56-byte stride): if its
 * condition is no longer active, stop it (looping sounds are explicitly stopped) and clear its handle/flag;
 * if active, ensure it is playing — looping sounds are started once and left running, impulse sounds are
 * (re)triggered on a rising edge (when the per-entry flag bit was not already set). sound_handles and
 * sound_flags persist the currently-playing state across calls.
 *
 * The entry's leading dword is a sound-group tag; 'lsnd' marks a looping sound (otherwise impulse).
 * Entry fields: type-mask@16, sound-definition index@12, scale@20. */

#include <stdint.h>
#include "headers/tag_block.h"

extern void unattached_looping_sound_stop(int looping_sound_index);
extern int unattached_looping_sound_start(int definition_index, int source_object_index, float scale);
extern void sound_stop_impulse(uint16_t sound_index);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void hud_play_sound(int16_t local_player_index, int type_flags, tag_block *sounds, int *sound_handles, uint16_t *sound_flags)
{
    if (sounds->count <= 0)
        return;

    for (int i = 0; i < sounds->count; i = (__int16)(i + 1))
    {
        char *entry = (char *)sounds->address + 56 * i;
        int looping = (*(unsigned int *)entry == 0x6C736E64u /* 'lsnd' */);
        int definition_index = ((int *)entry)[3];
        float scale = ((float *)entry)[5];

        if ((((int *)entry)[4] & type_flags) == 0)
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
