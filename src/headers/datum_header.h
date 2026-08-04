#pragma once
/* datum_header — leading bytes of every data_array element; identifier 0 == free slot. */

typedef struct datum_header
{
    __int16 identifier;  /* 0x0 */
} datum_header;
