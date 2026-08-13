/* recorded_animation_apply_event_stream_v1 @0x837F6C00 — version-1 playback codec's apply: consume every
 * event whose time delta has elapsed (each event is a 4-byte {type, time_delta} header): dispatch through
 * apply_funcs[type] (the handler consumes the event and advances *playback_stream itself; a NULL slot just
 * skips the 4-byte header), subtracting each consumed delta from *ticks. Event type 1 is the end-of-stream
 * marker: playback is finished (returns 0) exactly when the cursor sits on it with *ticks equal to its
 * delta; otherwise more remains (returns 1).
 *
 * DEVIATION: the decompiler inverted the dispatch call's argument list (calling the handler with only
 * `control`); disasm shows all three args (control, event, playback_stream) are passed. */

#include <stdint.h>
#include "headers/animation_playback_controller.h"
#include "headers/animation_event_v1.h"
#include "headers/playback_v1_event_type.h"

typedef struct unit_control_data unit_control_data;

extern void (*apply_funcs[])(unit_control_data *control, const animation_event_v1 *event,
        const char **playback_stream);

uint8_t recorded_animation_apply_event_stream_v1(animation_playback_controller *animation_state, unit_control_data *control, int *ticks, const char **playback_stream)
{
    const animation_event_v1 *event = (const animation_event_v1 *)*playback_stream;

    while ( *ticks >= event->time_delta )
    {
        if ( event->type == _playback_v1_end )
            break;

        void (*apply)(unit_control_data *, const animation_event_v1 *, const char **) =
            apply_funcs[event->type];
        if ( apply )
            apply(control, event, playback_stream);
        else
            *playback_stream = (const char *)(event + 1); /* skip the 4-byte event header (no operand) */

        *ticks -= event->time_delta;
        event = (const animation_event_v1 *)*playback_stream;
    }

    if ( event->type != _playback_v1_end )
        return 1;
    return *ticks == event->time_delta ? 0 : 1;
}
