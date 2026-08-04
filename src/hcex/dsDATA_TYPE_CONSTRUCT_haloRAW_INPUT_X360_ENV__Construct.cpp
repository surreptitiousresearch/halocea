#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/ds_new.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Construct @ 0x823BE720
// Box-construct: allocate a 0x18-byte (6-dword) haloRAW_INPUT_X360_ENV on the tracked heap and copy
// the default value in, word by word (reproduces the reversed pre-increment copy exactly; pointers
// start one word low). Store the pointer in the slot, or null on allocation failure.
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::Construct(
        unsigned int *storage, const haloRAW_INPUT_X360_ENV *defVal)
{
    void *box = operator new(0x18u, "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 0x225);
    unsigned int slot;
    if (box)
    {
        unsigned int *out = reinterpret_cast<unsigned int *>(box) - 1;
        const unsigned int *in = reinterpret_cast<const unsigned int *>(defVal) - 1;
        for (int count = 6; count; --count)
            *++out = *++in;
        slot = reinterpret_cast<unsigned int>(box); // reproduces the 32-bit boxed-pointer slot
    }
    else
    {
        slot = 0;
    }
    *storage = slot;
}
