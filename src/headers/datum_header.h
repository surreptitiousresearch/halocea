#pragma once
#include <stdint.h>
/* datum_header — leading bytes of every data_array element; identifier 0 == free slot. */

typedef struct datum_header
{
    int16_t identifier;  /* 0x0 */
} datum_header;
