#include <stdint.h>
#include "headers/blam_data_globals.h"
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* file_location_set_volume @0x837678C8 — copies volume_name into the 256-byte-stride slot for the given
 * file location, truncated and NUL-terminated at 255 characters. `file_location_volume_names` has no DB
 * type record; modeled as a flat byte array indexed by raw offset (location*256), matching the decompiler's
 * literal `[0][location*256]` 2D access — its outer index is always 0 in this function. */


extern char *strncpy(char *dst, const char *src, unsigned int count);

void file_location_set_volume(int16_t location, const char *volume_name)
{
    int offset = location << 8;
    /* canonical global is char[3][256]; index flat to preserve byte semantics */
    strncpy((char *)file_location_volume_names + offset, volume_name, 0xFFu);
    ((char *)file_location_volume_names)[offset + 255] = 0;
}
