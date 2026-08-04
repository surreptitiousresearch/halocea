#include "../headers/hcex/halo_ds_data_type_boundary.h"
#include "../headers/ws/ds/ds_new.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include <cstring>

// ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct @ 0x823BE5A0
// Box-construct: allocate a 0x44-byte haloPLAYER_ACTIONS_ENV on the tracked heap, copy the default
// value into it, and store the pointer in the slot (null slot on allocation failure).
template<>
void ds_data::dsDATA_TYPE_CONSTRUCT<haloPLAYER_ACTIONS_ENV>::Construct(
        unsigned int *storage, const haloPLAYER_ACTIONS_ENV *defVal)
{
    void *box = operator new(0x44u, "D:\\Projects\\code\\common\\INCL.SYS\\ds/ds_data_type_impl.hpp", 0x225);
    unsigned int slot;
    if (box)
    {
        memcpy(box, defVal, 0x44u);
        slot = reinterpret_cast<unsigned int>(box); // reproduces the 32-bit boxed-pointer slot
    }
    else
    {
        slot = 0;
    }
    *storage = slot;
}
