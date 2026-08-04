#include "../headers/byte_swap_definition.h"

/* .data init: byte_swap_definition for vector_char_difference_data. size=2, signature='bysw'. */
static int vector_char_difference_data_bs_codes[] = { _begin_bs_array, _1byte, _1byte, _1byte, _end_bs_array };

byte_swap_definition vector_char_difference_data_bs_definition =
{
    "vector_char_difference_data",                       /* name */
    2,                                        /* size */
    vector_char_difference_data_bs_codes,                /* codes */
    BYTE_SWAP_DEFINITION_SIGNATURE,            /* signature ('bysw') */
    0,                                         /* verified */
};
