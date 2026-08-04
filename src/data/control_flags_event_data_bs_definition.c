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
