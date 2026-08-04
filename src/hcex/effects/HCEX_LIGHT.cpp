// HCEX_LIGHT — a Blam flashlight light's ws-engine binding (weak entity handle + light pointer).
// POD-ish boilerplate: default/copy ctor, dtor, operator=. See src/headers/hcex/hcex_light.h for
// the DB-verified layout.
//
// DEVIATION: `ent` is modeled as the flat (non-template) hcex `ds_WEAK_PTR` -- a trivial struct
// with no declared destructor, so there is no implicit member destructor to lean on; the
// weak-handle refcount logic below is genuine, explicit body code, matching the disassembly.

#include "../../headers/hcex/hcex_light.h"

extern "C" int osLockedIncrement(int *pVar); // boundary -- src/ws/os/osLockedIncrement.cpp
extern "C" int osLockedDecrement(int *pVar); // boundary -- src/ws/os/osLockedDecrement.cpp

// 0x823D69AC — default-construct: null the weak-pointer handle. id/entLight are left
// uninitialized, matching the disassembly.
HCEX_LIGHT::HCEX_LIGHT()
{
    ent.pHandle = nullptr;
}

// 0x823D6120 — copy-construct: copy id, share that.ent's handle (ref-bump if non-null), copy
// entLight.
HCEX_LIGHT::HCEX_LIGHT(const HCEX_LIGHT &that)
{
    id = that.id;
    ds_WEAK_PTR_HANDLE *handle = that.ent.pHandle;
    ent.pHandle = handle;
    if (handle)
        osLockedIncrement(const_cast<int *>(&handle->refCount));
    entLight = that.entLight;
}

// 0x823D69CC — drop the weak-pointer handle's reference, operator-deleting it at zero, then null
// the handle.
HCEX_LIGHT::~HCEX_LIGHT()
{
    ds_WEAK_PTR_HANDLE *handle = ent.pHandle;
    if (handle)
    {
        osLockedDecrement(const_cast<int *>(&handle->refCount));
        if (handle->refCount <= 0)
        {
            ds_WEAK_PTR_HANDLE *toFree = ent.pHandle;
            if (toFree)
                operator delete(toFree);
        }
    }
    ent.pHandle = nullptr;
}

// 0x823D60B0 — copy-assign: copy id; if `ent` isn't already shared with `that.ent`, release
// this->ent's handle and adopt that.ent's handle (ref-bump if non-null); copy entLight.
HCEX_LIGHT &HCEX_LIGHT::operator=(const HCEX_LIGHT &that)
{
    id = that.id;
    if (&ent != &that.ent)
    {
        ds_WEAK_PTR_Release(&ent);
        ds_WEAK_PTR_HANDLE *handle = that.ent.pHandle;
        bool wasNull = (handle == nullptr);
        ent.pHandle = handle;
        if (!wasNull)
            osLockedIncrement(const_cast<int *>(&handle->refCount));
    }
    entLight = that.entLight;
    return *this;
}
