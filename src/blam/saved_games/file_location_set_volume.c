#include <stdint.h>
#include <string.h>
#include "headers/blam_data_globals.h"
/* file_location_set_volume @0x837678C8 — copies volume_name into the volume-name slot of the given file
 * location, truncated and NUL-terminated at 255 characters.
 * DEVIATION: the decompiler's flat `[0][location*256]` byte view is the real 2-D global —
 * applied_types gives `char file_location_volume_names[3][256]`, and the disassembly computes the row
 * base as `&file_location_volume_names + (location << 8)` (0x837678E4 slwi 8) with the terminator
 * written at row+0xFF (0x837678F8 addi r9, r31, 0xFF / stbx). */



void file_location_set_volume(int16_t location, const char *volume_name)
{
    strncpy(file_location_volume_names[location], volume_name, 0xFFu);
    file_location_volume_names[location][255] = 0;
}
