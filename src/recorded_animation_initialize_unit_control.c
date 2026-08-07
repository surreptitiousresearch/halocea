/* recorded_animation_initialize_unit_control @0x83826EA0 — decodes a recorded-animation playback stream into
 * a unit_control_data, applying every version's field table from 0 up through `unit_version` (or just version
 * 0 when unit_version is 0/1) cumulatively — old recordings only carry the fields their format version wrote,
 * so replaying a newer recording means walking every earlier version's table in order. Each table
 * (unit_control_data_map[version]) is a unit_control_data_entry array terminated by size == -1; a skipped
 * field (offset == -1) still advances the stream by its size without writing anywhere. */

#include <stdint.h>
#include <string.h>
#include "headers/unit_control_data.h"
#include "headers/unit_control_data_entry.h"
#include "headers/blam_data_globals.h"
/* memset declared by <string.h> */


void recorded_animation_initialize_unit_control(unit_control_data *control, const char **playback_stream, uint8_t unit_version)
{
    memset(control, 0, sizeof(unit_control_data));
    control->zoom_level = -1;

    unsigned int version_count = (unit_version <= 1) ? 1 : unit_version;

    for (unsigned int version = 0; version < version_count; version++)
    {
        for (unit_control_data_entry *entry = unit_control_data_map[version]; entry->size != -1; entry++)
        {
            if (entry->offset != -1)
                memcpy((char *)control + entry->offset, *playback_stream, entry->size);
            *playback_stream += entry->size;
        }
    }
}
