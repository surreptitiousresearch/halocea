#pragma once
#include "dscDESC.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
// boundary — dsc (descriptor system) mixin giving a runtime "product" object (e.g.
// gsOBJ_MODIFIER_BASE) an owning link back to its dscDESC descriptor. Non-polymorphic itself;
// a deriving class that introduces virtual functions gets its vfptr placed ahead of this
// sub-object under the MSVC ABI (see gsOBJ_MODIFIER_BASE.h).
// DB-verified layout (types_members dscPRODUCT): spDesc@0 — size 4.

typedef struct dscPRODUCT {
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x00
} dscPRODUCT;
