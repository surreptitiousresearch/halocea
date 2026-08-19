/* recorded_animation_initialize_event_stream @0x837F7388 — after initializing the unit control block from
 * the stream, load the full 12-byte animation_playback_controller verbatim from the stream and advance past it.
 *
 * DEVIATION: the decompiler renders the copy loop as `p_pitch = &animation_state[-1].looking_control.pitch`
 * plus six halfword stores — an artifact of pointer arithmetic on an opaque type. disasm_range(0x837F7388,
 * 0x837F73E0) shows a plain 6x __int16 copy from `*playback_stream` into `animation_state` (12 bytes total,
 * the whole struct) followed by `*playback_stream += 12`; reproduced as memcpy. */

#include <stdint.h>
#include <string.h>
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"

extern void recorded_animation_initialize_unit_control(unit_control_data *control, const char **playback_stream, uint8_t unit_version);

void recorded_animation_initialize_event_stream(animation_playback_controller *animation_state, unit_control_data *control, const char **playback_stream, uint8_t unit_control_version)
{
    recorded_animation_initialize_unit_control(control, playback_stream, unit_control_version);

    memcpy(animation_state, *playback_stream, 12); /* animation_playback_controller is opaque; DB size is 12 */
    *playback_stream += 12;
}
