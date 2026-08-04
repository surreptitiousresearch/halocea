/* unit_control_v4_map @ 0x84186730 -- second-order .data; unit_control_data_entry[2] reconstructed from binary bytes. */
/* NOTE: DB nominally types this as [10], but physical storage bounded by the adjacent symbol
 * (unit_control_data_map pointer table @ 0x84186748, only 24 bytes = 2 entries) is 2 terminated
 * entries; a [10] declaration (120 bytes) would overlap the pointer table — sized to [2]. 2026-07-31. */
#include "../headers/unit_control_data_entry.h"

extern byte_swap_definition _word_bs_definition;

unit_control_data_entry unit_control_v4_map[2] = {
    [0] = { .bs_def = &_word_bs_definition, .size = 2, .offset = 8 },
    [1] = { .size = -1, .offset = -1 }, /* table terminator */
};
