/* recorded_animation_write_unit_control @0x83826F78 — inverse of
 * recorded_animation_initialize_unit_control.c: serializes a unit_control_data into the recording stream,
 * applying every version's field table from 0 up through unit_version (or just version 0 when unit_version
 * is 0/1) cumulatively. Unlike the read path, this always copies (no offset==-1 skip check in the
 * decompiled body). */

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
