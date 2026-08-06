/* aiming_speed_event_data_bs_definition @ 0x841851F0 (.data, 20 bytes)
 * DB applied_types: byte_swap_definition aiming_speed_event_data_bs_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82128BA0 -> "aiming_speed_event_data"
 *   +0x04 size                       = 0x00000001
 *   +0x08 codes                      = 0x841851E0 -> aiming_speed_event_data_bs_codes
 *   +0x0C signature                  = 0x62797377
 *   +0x10 verified                   = 0x00
 */
#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for aiming_speed_event_data. size=1, signature='bysw'. */
static int aiming_speed_event_data_bs_codes[] = { _begin_bs_array, _1byte, _1byte, _end_bs_array };

byte_swap_definition aiming_speed_event_data_bs_definition =
{
    "aiming_speed_event_data",                       /* name */
    1,                                        /* size */
    aiming_speed_event_data_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
