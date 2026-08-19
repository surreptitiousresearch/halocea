/* scripted_looping_sound_stop @0x83714B78 — stop a scripted looping sound (immediate, no fade). */

#include <stdint.h>

/* extern aligned to the callee's definition (src/scripted_looping_sound_stop_internal.c;
 * unattested as of wave 1 — propagation will normalize) */
extern void scripted_looping_sound_stop_internal(int definition_index, uint8_t fixed_fadeout);

void scripted_looping_sound_stop(int definition_index)
{
    scripted_looping_sound_stop_internal(definition_index, 0);
}
