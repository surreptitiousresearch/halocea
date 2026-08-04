#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiPLANNER::IsWithinBox(m3dOBB&, aiBRAIN*) @ 0x83163C30  (protected)
// True when brain `br`'s world position lies inside the oriented box `box`.
bool aiPLANNER::IsWithinBox(const m3dOBB *box, aiBRAIN *br)
{
    // The brain's world position comes from a virtual at vtable byte-offset 0x1C0 that returns
    // an m3dV by value (sret): disasm calls (*(vtbl+448))(&pos, br). The exact method name is a
    // boundary (next frontier) — dispatched raw here to stay faithful to the object's vtable.
    typedef void (*GetPosFn)(m3dV *ret, aiBRAIN *self);
    void **vtbl = *reinterpret_cast<void ***>(br);
    GetPosFn getPos = reinterpret_cast<GetPosFn>(vtbl[0x1C0 / sizeof(void *)]);

    m3dV pos;
    getPos(&pos, br);
    return box->IsBelongPoint(&pos) != 0;
}
