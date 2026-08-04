#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for multi_vector_set_event_v1. size=16, signature='bysw'. */
static int multi_vector_set_event_v1_bs_codes[] = { _begin_bs_array, _1byte, _4byte, _4byte, _4byte, _end_bs_array };

byte_swap_definition multi_vector_set_event_v1_bs_definition =
{
    "multi_vector_set_event_v1",                       /* name */
    16,                                        /* size */
    multi_vector_set_event_v1_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
