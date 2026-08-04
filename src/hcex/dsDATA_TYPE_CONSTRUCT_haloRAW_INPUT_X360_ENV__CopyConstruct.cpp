#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/ds_new.h"
#include "../headers/hcex/haloRAW_INPUT_X360_ENV.h"

// ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyConstruct @ 0x823BE860
// Box-construct from another boxed slot: read the source box pointer via GetPtr, allocate a fresh
// 0x18-byte box, copy the value word by word (reproduces the reversed pre-increment copy exactly;
// pointers start one word low), and store the pointer (null slot on allocation failure).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::CopyConstruct(
        unsigned int *storage, const unsigned int *data)
{
    const haloRAW_INPUT_X360_ENV *src = ds_data::dsDATA_TYPE_CONSTRUCT<haloRAW_INPUT_X360_ENV>::GetPtr(data);
    void *box = operator new(0x18u, "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 0x233);
    unsigned int slot;
    if (box)
    {
        unsigned int *out = reinterpret_cast<unsigned int *>(box) - 1;
        const unsigned int *in = reinterpret_cast<const unsigned int *>(src) - 1;
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
