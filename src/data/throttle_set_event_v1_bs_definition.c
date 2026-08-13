/* throttle_set_event_v1_bs_definition @ 0x841850F8 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition throttle_set_event_v1_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82128A7C -> "throttle_set_event_v1"
 *   +0x04 size                       = 0x0000000C
 *   +0x08 codes                      = 0x841850E4 -> throttle_set_event_v1_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 */
#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for throttle_set_event_v1. size=12, signature='bysw'. */
static int throttle_set_event_v1_bs_codes[] = { _begin_bs_array, 1 /* array element count */, _4byte, _4byte, _end_bs_array };

byte_swap_definition throttle_set_event_v1_bs_definition =
{
    "throttle_set_event_v1",                       /* name */
    12,                                        /* size */
    throttle_set_event_v1_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
