/* recorded_animation_write_unit_control @0x83826F60 — inverse of
 * recorded_animation_initialize_unit_control.c: serializes a unit_control_data into the recording stream,
 * applying every version's field table from 0 up through unit_version (or just version 0 when unit_version
 * is 0/1) cumulatively.
 *
 * CAVEAT — shipped bug, faithfully reconstructed. Unlike the read path, this has no `offset == -1`
 * skip check: the loop tests only `size` (cmpwi cr6, r5, -1 @0x83826FB0 / @0x83826FDC), then loads
 * the offset unconditionally (lwz r11, 4(r31) @0x83826FB8), adds it to the struct base
 * (add r4, r11, r29 @0x83826FC0) and calls memcpy (@0x83826FC4). `unit_control_v1_map[4]` carries
 * size 2 / offset -1, so this serializes two bytes starting ONE byte before `unit_control_data` —
 * garbage in that stream field. The stream advance is unaffected (still +size), so the record layout
 * stays in step with the read path, which does skip offset -1. Do not add the guard. */

#include <stdint.h>
#include <string.h>
#include "headers/unit_control_data.h"
#include "headers/unit_control_data_entry.h"
#include "headers/blam_data_globals.h"


void recorded_animation_write_unit_control(unit_control_data *control, char **playback_stream, uint8_t unit_version)
{
    unsigned int version_count = (unit_version <= 1) ? 1 : unit_version;

    for ( unsigned int version = 0; version < version_count; version++ )
    {
        for ( unit_control_data_entry *entry = unit_control_data_map[version]; entry->size != -1; entry++ )
        {
            memcpy(*playback_stream, (char *)control + entry->offset, entry->size);
            *playback_stream += entry->size;
        }
    }
}
