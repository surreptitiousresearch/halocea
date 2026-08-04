/* unit_control_v1_map @ 0x84186688 -- second-order .data; unit_control_data_entry[10] reconstructed from binary bytes. */
#include "../headers/unit_control_data_entry.h"

extern byte_swap_definition _byte_bs_definition;
extern byte_swap_definition _word_bs_definition;
extern byte_swap_definition real_vector2d_bs_definition;
extern byte_swap_definition real_vector3d_bs_definition;

unit_control_data_entry unit_control_v1_map[10] = {
    [0] = { .bs_def = &_byte_bs_definition,          .size = 1,  .offset = 0 },
    [1] = { .bs_def = &_byte_bs_definition,          .size = 1,  .offset = 1 },
    [2] = { .bs_def = &_word_bs_definition,          .size = 2,  .offset = 2 },
    [3] = { .bs_def = &_word_bs_definition,          .size = 2,  .offset = 4 },
    [4] = { .bs_def = &_word_bs_definition,          .size = 2,  .offset = -1 },
    [5] = { .bs_def = &real_vector2d_bs_definition,  .size = 8,  .offset = 12 },
    [6] = { .bs_def = &real_vector3d_bs_definition,  .size = 12, .offset = 28 },
    [7] = { .bs_def = &real_vector3d_bs_definition,  .size = 12, .offset = 40 },
    [8] = { .bs_def = &real_vector3d_bs_definition,  .size = 12, .offset = 52 },
    [9] = { .size = -1, .offset = -1 }, /* table terminator (size == -1) */
};
