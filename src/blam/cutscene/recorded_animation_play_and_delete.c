/* recorded_animation_play_and_delete @0x83712600 — play a recorded animation, then delete the unit when
 * it finishes. Thin wrapper: forwards to recorded_animation_play_internal with the delete flag. */

#include <stdint.h>
#include "headers/recording_thread_flags.h"

extern uint8_t recorded_animation_play_internal(int unit_index, int16_t animation_index, uint16_t extra_flags);

uint8_t recorded_animation_play_and_delete(int unit_index, int16_t animation_index)
{
    return recorded_animation_play_internal(unit_index, animation_index,
        1u << _recording_thread_delete_unit_on_complete_bit);
}
