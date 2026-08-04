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
