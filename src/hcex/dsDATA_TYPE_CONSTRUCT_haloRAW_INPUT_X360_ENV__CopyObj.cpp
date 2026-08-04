#include "../headers/ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyObj @ 0x823BCF10
// Overwrite the boxed 24-byte (6-dword) haloRAW_INPUT_X360_ENV at *dest from the raw object
// `obj`. The source pointer starts one word low (decompiler: &obj[-1].crc == (int*)obj - 1) so
// the pre-increment lands on obj+0; reproduced faithfully.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyObj(
        const haloRAW_INPUT_X360_ENV *obj, unsigned int *dest)
{
    unsigned int *out = reinterpret_cast<unsigned int *>(*dest) - 1;
    const unsigned int *in = reinterpret_cast<const unsigned int *>(obj) - 1;
    for (int count = 6; count; --count)
        *++out = *++in;
}
