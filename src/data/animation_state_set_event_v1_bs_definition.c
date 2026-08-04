#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for animation_state_set_event_v1. size=6, signature='bysw'. */
static int animation_state_set_event_v1_bs_codes[] = { _begin_bs_array, _1byte, _1byte, _end_bs_array };

byte_swap_definition animation_state_set_event_v1_bs_definition =
{
    "animation_state_set_event_v1",                       /* name */
    6,                                        /* size */
    animation_state_set_event_v1_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
