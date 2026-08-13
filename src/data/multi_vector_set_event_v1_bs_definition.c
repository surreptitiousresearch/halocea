/* multi_vector_set_event_v1_bs_definition @ 0x84185124 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition multi_vector_set_event_v1_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82128A60 -> "multi_vector_set_event_v1"
 *   +0x04 size                       = 0x00000010
 *   +0x08 codes                      = 0x8418510C -> multi_vector_set_event_v1_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 */
#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for multi_vector_set_event_v1. size=16, signature='bysw'. */
static int multi_vector_set_event_v1_bs_codes[] = { _begin_bs_array, 1 /* array element count */, _4byte, _4byte, _4byte, _end_bs_array };

byte_swap_definition multi_vector_set_event_v1_bs_definition =
{
    "multi_vector_set_event_v1",                       /* name */
    16,                                        /* size */
    multi_vector_set_event_v1_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
