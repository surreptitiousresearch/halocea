#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/ds_new.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyConstruct @ 0x823BE658
// Box-construct from another boxed slot: read the source box pointer via GetPtr, allocate a fresh
// 0x44-byte box, copy the value, and store the pointer (null slot on allocation failure).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::CopyConstruct(
        unsigned int *storage, const unsigned int *data)
{
    const haloPLAYER_ACTIONS_ENV *src = ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::GetPtr(data);
    void *box = operator new(0x44u, "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 0x233);
    unsigned int slot;
    if (box)
    {
        memcpy(box, src, 0x44u);
        slot = reinterpret_cast<unsigned int>(box); // reproduces the 32-bit boxed-pointer slot
    }
    else
    {
        slot = 0;
    }
    *storage = slot;
}
