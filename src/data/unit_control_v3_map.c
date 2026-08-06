/* unit_control_v3_map @ 0x84186718 (.data, 24 bytes)
 * DB applied_types: unit_control_data_entry unit_control_v3_map[2];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 bs_def                     = 0x84185764 -> _word_bs_definition
 *        +0x04 size                       = 0x00000002
 *        +0x08 offset                     = 0x00000006
 *   [ 1] +0x00 bs_def                     = 0x00000000
 *        +0x04 size                       = 0xFFFFFFFF
 *        +0x08 offset                     = 0xFFFFFFFF
 * second-order .data; unit_control_data_entry[2] reconstructed from binary bytes.
 */
/* NOTE: DB nominally types this as [10], but physical storage bounded by the adjacent symbol
 * (unit_control_v4_map @ 0x84186730, only 24 bytes = 2 entries) is 2 terminated entries; a [10]
 * declaration (120 bytes) would overlap v4_map — sized to the ground-truth [2]. 2026-07-31. */
#include "../headers/unit_control_data_entry.h"

extern byte_swap_definition _word_bs_definition;

unit_control_data_entry unit_control_v3_map[2] = {
    [0] = { .bs_def = &_word_bs_definition, .size = 2, .offset = 6 },
    [1] = { .size = -1, .offset = -1 }, /* table terminator */
};
