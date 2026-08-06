/* long_bs_definition @ 0x84185788 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition long_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82118E94 -> "long"
 *   +0x04 size                       = 0x00000004
 *   +0x08 codes                      = 0x84185778 -> long_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include "byte_swap_definition.h"

extern int long_bs_codes[];

byte_swap_definition long_bs_definition =
{
    .name      = "long",                          /* name */
    .size      = 4,                               /* size */
    .codes     = long_bs_codes,                   /* codes */
    .signature = BYTE_SWAP_DEFINITION_SIGNATURE,  /* signature */
    .verified  = 0,                               /* verified */
};
