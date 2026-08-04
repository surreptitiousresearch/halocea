// dsPAIR<int,HCEX_LIGHT>::dsPAIR(const dsPAIR<int,HCEX_LIGHT> &that) @ 0x823D8538
//
// Copy-construct one (key, HCEX_LIGHT) light-binding entry. The compiler inlined the HCEX_LIGHT
// member copy-ctor: copy id, share that.val.ent's weak handle (ref-bump if non-null), copy
// entLight. Reversed per-instantiation.

#include "../headers/hcex/hcex_light.h"

extern "C" int osLockedIncrement(int *pVar); // boundary -- src/ws/os/osLockedIncrement.cpp

template<>
hcex_light_pair::dsPAIR(const hcex_light_pair &that)
{
    this->key = that.key;

    // inlined HCEX_LIGHT copy-ctor:
    this->val.id = that.val.id;
    ds_WEAK_PTR_HANDLE *handle = that.val.ent.pHandle;
    this->val.ent.pHandle = handle;
    if (handle)
        osLockedIncrement(const_cast<int *>(&handle->refCount));
    this->val.entLight = that.val.entLight;
}
