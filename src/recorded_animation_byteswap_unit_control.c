/* recorded_animation_byteswap_unit_control @0x83826E14 — byte-swaps a unit_control_data record already written
 * into the recording stream, in place, applying every version's field table from 0 up through unit_version (or
 * just version 0 when unit_version is 0/1) cumulatively — same version-table walk as
 * recorded_animation_write_unit_control.c, but swapping bytes at the stream cursor instead of copying from a
 * live struct.
 *
 * Deviation: the decompiler's pointer-aliasing (`i` retargeted mid-loop to the not-yet-incremented entry's
 * `size` field) obscures a plain "swap this entry's bytes, then advance the cursor by its size" loop; simplified
 * to that form, matching recorded_animation_write_unit_control.c's structure. */

#include <stdint.h>
#include "headers/unit_control_data.h"
#include "headers/unit_control_data_entry.h"
#include "headers/blam_data_globals.h"

extern void byte_swap_data(byte_swap_definition *definition, void *data, int data_count);

void recorded_animation_byteswap_unit_control(char **playback_stream, uint8_t unit_version)
{
    unsigned int version_count = (unit_version <= 1) ? 1 : unit_version;

    for ( unsigned int version = 0; version < version_count; version++ )
    {
        for ( unit_control_data_entry *entry = unit_control_data_map[version]; entry->size != -1; entry++ )
        {
            byte_swap_data(entry->bs_def, *playback_stream, 1);
            *playback_stream += entry->size;
        }
    }
}
