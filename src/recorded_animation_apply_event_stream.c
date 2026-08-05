/* recorded_animation_apply_event_stream @0x837F7478 — default (non-v1) playback codec's apply: consume
 * packed-byte events from `playback_stream` whose time delta has elapsed. Each event's leading byte packs
 * a 2-bit delta-width tag (0/1 = implicit delta 1, header is 1 byte; 2 = delta in the next byte, header 2
 * bytes; 3 = delta in the next 2 bytes, header 3 bytes) in bits 0-1, and an event id in bits 2-7 (id 1 is
 * the end-of-stream marker) used as `apply_funcs_0[id]` — the id is already byte-scaled via `byte & 0xFC`
 * since apply_funcs_0 is a table of 4-byte function pointers. The stream cursor is advanced before the
 * handler runs (handlers here take just animation_state/control, unlike the v1 codec's handlers which
 * advance the cursor themselves). Playback is finished (returns 0) exactly when the cursor sits on the end
 * marker with *ticks equal to its delta; otherwise more remains (returns 1). Sibling of
 * recorded_animation_apply_event_stream_v1.c. */
#include <stdint.h>

#include "headers/animation_playback_controller.h"
#include "headers/recorded_animation_time_delta.h"

typedef struct unit_control_data unit_control_data;

extern void (*apply_funcs_0[])(animation_playback_controller *animation_state, unit_control_data *control);

uint8_t recorded_animation_apply_event_stream(animation_playback_controller *animation_state,
        unit_control_data *control, int *ticks, const char **playback_stream)
{
    const unsigned char *event;
    unsigned int event_byte;
    uint16_t delta;
    uint8_t header_size;

    while ( 1 )
    {
        event = (const unsigned char *)*playback_stream;
        event_byte = *event;

        switch ( event_byte & 3 )
        {
        case _time_delta_one:
            delta = 1;
            header_size = 1;
            break;
        case _time_delta_byte:
            delta = event[1];
            header_size = 2;
            break;
        case _time_delta_zero:
            delta = 0;
            header_size = 1;
            break;
        default: /* _time_delta_word */
            /* DEVIATION: decompiled as `*(const uint16_t *)(event + 1)`. The event stream is
             * serialized big-endian, so that spelling only yields the right value on PPC — and it
             * is an unaligned 16-bit load at an odd byte offset. Combine the bytes explicitly. */
            delta = (uint16_t)((event[1] << 8) | event[2]);
            header_size = 3;
            break;
        }

        /* event id field = bits 2-7 (~0x3 masks off the 2-bit delta-width tag); id 1 (byte-scaled 4) = end marker */
        if ( *ticks < delta || (event_byte & ~0x3u) == 4 )
            break;

        *playback_stream = (const char *)event + header_size;

        void (*apply)(animation_playback_controller *, unit_control_data *) =
            apply_funcs_0[(event_byte & ~0x3u) >> 2];
        if ( apply )
            apply(animation_state, control);

        *ticks -= delta;
    }

    if ( (event_byte & ~0x3u) != 4 )
        return 1;
    return *ticks == delta ? 0 : 1;
}
