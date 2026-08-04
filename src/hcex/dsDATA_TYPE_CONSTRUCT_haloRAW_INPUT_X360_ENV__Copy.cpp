#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Copy @ 0x823BCE48
// Overwrite the boxed 24-byte (6-dword) haloRAW_INPUT_X360_ENV at *dest from the one at *src.
// Reproduces the reversed pre-increment word copy exactly (pointers start one word low).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Copy(
        const unsigned int *src, unsigned int *dest)
{
    unsigned int *out = reinterpret_cast<unsigned int *>(*dest) - 1;
    const unsigned int *in = reinterpret_cast<const unsigned int *>(*src) - 1;
    for (int count = 6; count; --count)
        *++out = *++in;
}
