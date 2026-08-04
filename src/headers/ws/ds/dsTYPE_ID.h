#pragma once
#include "dsFIXED_BIT_ARRAY.h"
// ws-engine ds: RTTI-style type identifier — a fixed bitmask encoding a type's position in
// the derivation lattice. A type is derived from another iff its base's bits are a subset of
// its own bits, checked cell by cell.
// DB-verified layout (types_members dsTYPE_ID): id@0 (dsFIXED_BIT_ARRAY<4>) — size 16.

typedef struct dsTYPE_ID {
    dsFIXED_BIT_ARRAY<4> id; // 0x00 derivation bitmask (4 x 32-bit cells)

    // 0x826C1710 — true iff this type-id is derived from `baseTypeID`, i.e. every set bit of
    // baseTypeID.id is also set in this->id (asserts baseTypeID is initialised first).
    int IsDerivedFrom(const dsTYPE_ID &baseTypeID) const;

    // True iff any bit is set (the id has been assigned); body not decompiled here.
    bool IsInited() const; // boundary
} dsTYPE_ID;
