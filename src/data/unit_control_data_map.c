/* unit_control_data_map @ 0x84186748 (.data, 16 bytes)
 * DB applied_types: unit_control_data_entry *unit_control_data_map[4];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x84186688 -> unit_control_v1_map
 *   +0x0004 = 0x84186700 -> unit_control_v2_map
 *   +0x0008 = 0x84186718 -> unit_control_v3_map
 *   +0x000C = 0x84186730 -> unit_control_v4_map
 * .data data-init.
 * DB: unit_control_data_entry *[4]; pointer table into the four per-version
 * unit-control maps (reloc order: v1,v2,v3,v4).
 */
#include "headers/unit_control_data_entry.h"

extern unit_control_data_entry unit_control_v1_map;
extern unit_control_data_entry unit_control_v2_map;
extern unit_control_data_entry unit_control_v3_map;
extern unit_control_data_entry unit_control_v4_map;

unit_control_data_entry *unit_control_data_map[4] = {
    &unit_control_v1_map,
    &unit_control_v2_map,
    &unit_control_v3_map,
    &unit_control_v4_map,
};
