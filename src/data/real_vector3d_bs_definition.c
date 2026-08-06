/* real_vector3d_bs_definition @ 0x84186670 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition real_vector3d_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82129F68 -> "real_vector3d"
 *   +0x04 size                       = 0x0000000C
 *   +0x08 codes                      = 0x84186658 -> real_vector3d_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include "byte_swap_definition.h"

extern int real_vector3d_bs_codes[];

byte_swap_definition real_vector3d_bs_definition =
{
    .name      = "real_vector3d",
    .size      = 12, /* 0x0C */
    .codes     = real_vector3d_bs_codes,
    .signature = BYTE_SWAP_DEFINITION_SIGNATURE,
    .verified  = 0,
};
