/* apply_funcs_0 @ 0x84185160, 92 bytes (data-init; ledger mislabeled data-bss).
   recorded-animation event-apply dispatch table (sparse). Targets recovered
   from binary relocations. */
#include "../headers/animation_playback_controller.h"
#include "../headers/unit_control_data.h"
#include "../headers/animation_event_header.h"

extern void apply_aiming_speed_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_animation_state_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_control_flags_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_throttle_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_vector_char_difference(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_vector_short_difference(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);
extern void apply_weapon_index_0(animation_playback_controller *animation_state, unit_control_data *control, const animation_event_header *header, const char **playback_stream);

void (__fastcall *apply_funcs_0[23])(animation_playback_controller *, unit_control_data *, const animation_event_header *, const char **) = {
    [2] = apply_animation_state_0,
    [3] = apply_aiming_speed_0,
    [4] = apply_control_flags_0,
    [5] = apply_weapon_index_0,
    [6] = apply_throttle_0,
    [7] = apply_vector_char_difference,
    [8] = apply_vector_char_difference,
    [9] = apply_vector_char_difference,
    [10] = apply_vector_char_difference,
    [11] = apply_vector_char_difference,
    [12] = apply_vector_char_difference,
    [13] = apply_vector_char_difference,
    [14] = apply_vector_char_difference,
    [15] = apply_vector_short_difference,
    [16] = apply_vector_short_difference,
    [17] = apply_vector_short_difference,
    [18] = apply_vector_short_difference,
    [19] = apply_vector_short_difference,
    [20] = apply_vector_short_difference,
    [21] = apply_vector_short_difference,
    [22] = apply_vector_short_difference,
};
