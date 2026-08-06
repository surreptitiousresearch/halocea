/* apply_funcs @ 0x84184FD0 (.data, 92 bytes)
 * DB applied_types: void (__fastcall *apply_funcs[23])(unit_control_data *, const animation_event_v1 *, const char **);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x837F69D8 -> apply_animation_state
 *   +0x000C = 0x837F69F0 -> apply_aiming_speed
 *   +0x0010 = 0x837F6A08 -> apply_control_flags
 *   +0x0014 = 0x837F6A20 -> apply_weapon_index
 *   +0x0018 = 0x837F6A38 -> apply_throttle
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x837F6A68 -> apply_facing_vector
 *   +0x0028 = 0x837F6A90 -> apply_aiming_vector
 *   +0x002C = 0x837F6AB8 -> apply_looking_vector
 *   +0x0030 = 0x837F6B70 -> apply_multi_vector
 *   +0x0034 = 0x837F6B70 -> apply_multi_vector
 *   +0x0038 = 0x837F6B70 -> apply_multi_vector
 *   +0x003C = 0x837F6B70 -> apply_multi_vector
 *   +0x0040 = 0x837F6AE0 -> apply_angle_vector
 *   +0x0044 = 0x837F6AE0 -> apply_angle_vector
 *   +0x0048 = 0x837F6AE0 -> apply_angle_vector
 *   +0x004C = 0x837F6AE0 -> apply_angle_vector
 *   +0x0050 = 0x837F6AE0 -> apply_angle_vector
 *   +0x0054 = 0x837F6AE0 -> apply_angle_vector
 *   +0x0058 = 0x837F6AE0 -> apply_angle_vector
 * 92 bytes (data-init; ledger mislabeled data-bss).
 *    recorded-animation v1 event-apply dispatch table (sparse). Targets recovered
 *    from binary relocations.
 */
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

void (*apply_funcs[23])(unit_control_data *, const animation_event_v1 *, const char **) = {
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
