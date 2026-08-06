/* _word_bs_definition @ 0x84185764 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition word_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82025468 -> "word"
 *   +0x04 size                       = 0x00000002
 *   +0x08 codes                      = 0x84185754 -> _word_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 * third-order .data (data_closure.py); reconstructed from binary bytes.
 */
#include "byte_swap_definition.h"

extern int _word_bs_codes[];

byte_swap_definition _word_bs_definition =
{
    .name      = "word",                          /* name */
    .size      = 2,                               /* size */
    .codes     = _word_bs_codes,                  /* codes */
    .signature = BYTE_SWAP_DEFINITION_SIGNATURE,  /* signature */
    .verified  = 0,                               /* verified */
};
