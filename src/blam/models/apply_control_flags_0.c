/* apply_control_flags_0 @0x837F6EC8 */
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/animation_event_header.h"

void apply_control_flags_0(animation_playback_controller *animation_state,
                            unit_control_data *control,
                            const animation_event_header *header,
                            const char **playback_stream)
{
    control->control_flags = *(unsigned short *)*playback_stream;
    *playback_stream += 2;
}
