/* recorded_animation_play_and_hover @0x83712608 — play a recorded animation on a vehicle and keep it
 * hovering. Thin wrapper: forwards to recorded_animation_play_internal with the hover flag. */

#include <stdint.h>
#include "headers/recording_thread_flags.h"

extern uint8_t recorded_animation_play_internal(int unit_index, int16_t animation_index, uint16_t extra_flags);

uint8_t recorded_animation_play_and_hover(int vehicle_index, int16_t animation_index)
{
    return recorded_animation_play_internal(vehicle_index, animation_index,
        1u << _recording_thread_hover_vehicle_on_complete_bit);
}
