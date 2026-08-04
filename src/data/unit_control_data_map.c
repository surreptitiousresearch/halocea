/* unit_control_data_map @ 0x84186748 — .data data-init.
 * DB: unit_control_data_entry *[4]; pointer table into the four per-version
 * unit-control maps (reloc order: v1,v2,v3,v4). */
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
