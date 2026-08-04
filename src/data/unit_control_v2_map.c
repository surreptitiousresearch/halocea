/* unit_control_v2_map @ 0x84186700 -- second-order .data; unit_control_data_entry[2] reconstructed from binary bytes. */
/* NOTE: DB nominally types this as [10], but physical storage bounded by the adjacent symbol
 * (unit_control_v3_map @ 0x84186718, only 24 bytes = 2 entries) is 2 terminated entries; a [10]
 * declaration (120 bytes) would overlap v3/v4_map — sized to the ground-truth [2]. 2026-07-31. */
#include "../headers/unit_control_data_entry.h"

extern byte_swap_definition long_bs_definition;

unit_control_data_entry unit_control_v2_map[2] = {
    [0] = { .bs_def = &long_bs_definition, .size = 4, .offset = 24 },
    [1] = { .size = -1, .offset = -1 }, /* table terminator */
};
