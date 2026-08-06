/* real_vector2d_bs_definition @ 0x84186644 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition real_vector2d_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82129F78 -> "real_vector2d"
 *   +0x04 size                       = 0x00000008
 *   +0x08 codes                      = 0x84186630 -> real_vector2d_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include "byte_swap_definition.h"

extern int real_vector2d_bs_codes[];

byte_swap_definition real_vector2d_bs_definition =
{
    .name      = "real_vector2d",                 /* name */
    .size      = 8,                               /* size */
    .codes     = real_vector2d_bs_codes,          /* codes */
    .signature = BYTE_SWAP_DEFINITION_SIGNATURE,  /* signature */
    .verified  = 0,                               /* verified */
};
