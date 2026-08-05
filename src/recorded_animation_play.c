/* recorded_animation_play @0x837125F8 — start playing a recorded animation on a unit.
 * Thin wrapper: forwards to recorded_animation_play_internal with no extra flags. */

#include <stdint.h>

extern uint8_t recorded_animation_play_internal(int unit_index, int16_t animation_index, uint16_t extra_flags);

uint8_t recorded_animation_play(int unit_index, int16_t animation_index)
{
    return recorded_animation_play_internal(unit_index, animation_index, 0);
}
