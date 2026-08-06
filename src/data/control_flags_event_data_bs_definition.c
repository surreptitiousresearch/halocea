/* control_flags_event_data_bs_definition @ 0x84185214 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition control_flags_event_data_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82128B84 -> "control_flags_event_data"
 *   +0x04 size                       = 0x00000002
 *   +0x08 codes                      = 0x84185204 -> control_flags_event_data_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 */
#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for control_flags_event_data. size=2, signature='bysw'. */
static int control_flags_event_data_bs_codes[] = { _begin_bs_array, _1byte, _2byte, _end_bs_array };

byte_swap_definition control_flags_event_data_bs_definition =
{
    "control_flags_event_data",                       /* name */
    2,                                        /* size */
    control_flags_event_data_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
