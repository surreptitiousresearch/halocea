/* unit_control_v1_map @ 0x84186688 (.data, 120 bytes)
 * DB applied_types: unit_control_data_entry unit_control_v1_map[10];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 bs_def                     = 0x84185740 -> _byte_bs_definition
 *        +0x04 size                       = 0x00000001
 *        +0x08 offset                     = 0x00000000
 *   [ 1] +0x00 bs_def                     = 0x84185740 -> _byte_bs_definition
 *        +0x04 size                       = 0x00000001
 *        +0x08 offset                     = 0x00000001
 *   [ 2] +0x00 bs_def                     = 0x84185764 -> _word_bs_definition
 *        +0x04 size                       = 0x00000002
 *        +0x08 offset                     = 0x00000002
 *   [ 3] +0x00 bs_def                     = 0x84185764 -> _word_bs_definition
 *        +0x04 size                       = 0x00000002
 *        +0x08 offset                     = 0x00000004
 *   [ 4] +0x00 bs_def                     = 0x84185764 -> _word_bs_definition
 *        +0x04 size                       = 0x00000002
 *        +0x08 offset                     = 0xFFFFFFFF
 *   [ 5] +0x00 bs_def                     = 0x84186644 -> real_vector2d_bs_definition
 *        +0x04 size                       = 0x00000008
 *        +0x08 offset                     = 0x0000000C
 *   [ 6] +0x00 bs_def                     = 0x84186670 -> real_vector3d_bs_definition
 *        +0x04 size                       = 0x0000000C
 *        +0x08 offset                     = 0x0000001C
 *   [ 7] +0x00 bs_def                     = 0x84186670 -> real_vector3d_bs_definition
 *        +0x04 size                       = 0x0000000C
 *        +0x08 offset                     = 0x00000028
 *   [ 8] +0x00 bs_def                     = 0x84186670 -> real_vector3d_bs_definition
 *        +0x04 size                       = 0x0000000C
 *        +0x08 offset                     = 0x00000034
 *   [ 9] +0x00 bs_def                     = 0x00000000
 *        +0x04 size                       = 0xFFFFFFFF
 *        +0x08 offset                     = 0xFFFFFFFF
 * second-order .data; unit_control_data_entry[10] reconstructed from binary bytes.
 */
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
