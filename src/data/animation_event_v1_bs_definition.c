/* animation_event_v1_bs_definition @ 0x84185040 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition animation_event_v1_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82128B08 -> "animation_event_v1"
 *   +0x04 size                       = 0x00000004
 *   +0x08 codes                      = 0x8418502C -> animation_event_v1_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 */
#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for animation_event_v1. size=4, signature='bysw'. */
static int animation_event_v1_bs_codes[] = { _begin_bs_array, _1byte, _2byte, _2byte, _end_bs_array };

byte_swap_definition animation_event_v1_bs_definition =
{
    "animation_event_v1",                       /* name */
    4,                                        /* size */
    animation_event_v1_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
