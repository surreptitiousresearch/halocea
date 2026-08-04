/* _byte_swap_recording_stream_v1 @0x837F6CC8 — byte-swap a version-1 recording (replay) stream in place. It
 * first swaps the leading unit_control block via recorded_animation_byteswap_unit_control, then swaps the
 * stream header with the animation-event byte-swap table, and finally walks the chain of animation-event
 * records: each record begins with a 16-bit version tag; while that tag is 0 the record's payload is swapped
 * and the cursor advances one record (4 bytes), stopping when the tag reaches 1 (end) or any other value.
 *
 * DEVIATION: the DB prototype is void byte_swap_recording_stream_v1(void *data, int size,
 * unsigned __int8 unit_control_version); Hex-Rays invented eight phantom trailing int args (a4..a11) and a
 * reused local (a12) from uninitialized argument registers — disasm (0x837F6CD4) shows only r3/r5 are used
 * (r4/size is unreferenced). The `while(2)`/switch idiom is a single tag-driven loop, rewritten as such. */

#include <stdint.h>
#include "headers/byte_swap_definition.h"
#include "headers/blam_data_globals.h"

extern void recorded_animation_byteswap_unit_control(char **playback_stream, uint8_t unit_version);
extern void byte_swap_data(byte_swap_definition *definition, void *data, int data_count);

void _byte_swap_recording_stream_v1(void *data, int size, uint8_t unit_control_version)
{
    char *playback_stream = (char *)data;
    recorded_animation_byteswap_unit_control(&playback_stream, unit_control_version);

    byte_swap_data(&animation_event_v1_bs_definition, data, 1);

    unsigned __int16 version = *(unsigned __int16 *)data;
    unsigned __int16 *cursor = (unsigned __int16 *)((char *)data + 4);
    while ( version != 1 )
    {
        if ( (__int16)version != 0 )
            break;
        byte_swap_data(&animation_event_v1_bs_definition, cursor, 1);
        version = *cursor;
        cursor += 2;
    }
}
