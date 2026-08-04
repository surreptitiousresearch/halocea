/* recorded_animation_initialize_event_stream_with_size @0x837F73F8 — read the full unit_control_data (64
 * bytes) verbatim from the stream, then the 12-byte animation_playback_controller verbatim, advancing past
 * each in turn. Same DEVIATION as recorded_animation_initialize_event_stream.c: the decompiler's
 * `p_pitch = &animation_state[-1].looking_control.pitch` is an artifact of pointer arithmetic on an opaque
 * type; disasm_range(0x837F73F8,0x837F745C) shows a plain 12-byte memcpy. */

#include <string.h>
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"

void recorded_animation_initialize_event_stream_with_size(animation_playback_controller *animation_state,
    unit_control_data *control, const char **playback_stream)
{
    memcpy(control, *playback_stream, sizeof(unit_control_data));
    *playback_stream += sizeof(unit_control_data);

    memcpy(animation_state, *playback_stream, 12); /* animation_playback_controller is opaque; DB size is 12 */
    *playback_stream += 12;
}
