#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for throttle_event_data. size=8, signature='bysw'. */
static int throttle_event_data_bs_codes[] = { _begin_bs_array, _1byte, _4byte, _4byte, _end_bs_array };

byte_swap_definition throttle_event_data_bs_definition =
{
    "throttle_event_data",                       /* name */
    8,                                        /* size */
    throttle_event_data_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
