/* apply_funcs @ 0x84184FD0, 92 bytes (data-init; ledger mislabeled data-bss).
   recorded-animation v1 event-apply dispatch table (sparse). Targets recovered
   from binary relocations. */
#include "../headers/unit_control_data.h"
#include "../headers/animation_event_v1.h"

extern void apply_aiming_speed(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_aiming_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_angle_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_animation_state(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_control_flags(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_facing_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_looking_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_multi_vector(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_throttle(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);
extern void apply_weapon_index(unit_control_data *control, const animation_event_v1 *anim_event_v1, const char **playback_stream);

void (__fastcall *apply_funcs[23])(unit_control_data *, const animation_event_v1 *, const char **) = {
    [2] = apply_animation_state,
    [3] = apply_aiming_speed,
    [4] = apply_control_flags,
    [5] = apply_weapon_index,
    [6] = apply_throttle,
    [9] = apply_facing_vector,
    [10] = apply_aiming_vector,
    [11] = apply_looking_vector,
    [12] = apply_multi_vector,
    [13] = apply_multi_vector,
    [14] = apply_multi_vector,
    [15] = apply_multi_vector,
    [16] = apply_angle_vector,
    [17] = apply_angle_vector,
    [18] = apply_angle_vector,
    [19] = apply_angle_vector,
    [20] = apply_angle_vector,
    [21] = apply_angle_vector,
    [22] = apply_angle_vector,
};
