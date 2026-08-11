#include <stdint.h>
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* _byte_swap_recording @0x8381F830 — byte-swap a chunk of recording data, dispatched by the recording
 * format version stored at raw offset 32 within the recording header (byte offsets 32/34, no DB struct
 * name established for the recording header). Versions 1-3 use the legacy stream swapper; version 4+
 * uses the current one. Version 0 (no recording) is a no-op. */

extern void _byte_swap_recording_stream_v1(void *data, int size, uint8_t unit_control_version);
extern void _byte_swap_recording_stream(char *data, int size, uint8_t unit_control_version);

void _byte_swap_recording(unsigned char *recording, void *data, int size)
{
    unsigned char version = recording[32];
    if ( !version )
        return;
    if ( version <= 3 )
        _byte_swap_recording_stream_v1(data, size, recording[34]);
    else if ( version == 4 )
        _byte_swap_recording_stream(data, size, recording[34]);
}
