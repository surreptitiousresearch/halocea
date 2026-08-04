#include "../../headers/ws/ai/aiBODY_IFACE.h"

// 0x832ABFE0 ?GetRefPropCache@aiBODY_IFACE@@UBAABUctrlPROP_LIST@@XZ
//
// This is the aiBODY_IFACE base (default) implementation for vtbl slot 35. The class method
// GetRefPropCache is modeled in aiBODY_IFACE.h as an inline vtable dispatcher: the hierarchy uses an
// explicit `__vftable` member (not C++ `virtual`), so a second out-of-line member of the same name
// cannot coexist with the dispatcher. This free function reproduces the base slot body faithfully:
// it returns a process-wide empty ctrlPROP_LIST, zero-initialised once on first use (the binary
// memsets a guard-protected static to 0 — static storage is already zero-initialised).
const ctrlPROP_LIST *aiBODY_IFACE_GetRefPropCache_base(const aiBODY_IFACE *self)
{
    (void)self;
    static ctrlPROP_LIST s_emptyPropList; // zero-initialised (static storage)
    return &s_emptyPropList;
}
