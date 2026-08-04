#pragma once
#include "dscBRAND.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/MAP.h"
#include "../ps/psSECTION.h"
// boundary — dsc (descriptor system) family: the per-brand-type registry (owns the sorted
// brand list, the pending-config-section map brands pull from during Init, and a name-ordered
// registration list). FindBrand and IsDerivedFrom are reversed in full; the rest of the class
// is reproduced faithfully from the DB layout but external to this batch.
// DB-verified layout (types_members dscDESC_FAMILY):
//   __vftable@0, isReplaceAllowed@4, psList@8 (84B), brandList@92 (20B), orderedList@112 (20B),
//   name@132, isAllowReload@136 — size >= 137 (padded).

// DB-verified (types_members dscDESC_FAMILY_vtbl), size 24 (6 slots).
typedef struct dscDESC_FAMILY_vtbl {
    void            (*dtr_dscDESC_FAMILY)(dscDESC_FAMILY *self);                       // 0x00
    void            (*PostRegisterBrands)(dscDESC_FAMILY *self);                        // 0x04
    dsTSTRING<char> *(*GetExtension)(dscDESC_FAMILY *self, dsTSTRING<char> *result);     // 0x08
    void            (*Init)(dscDESC_FAMILY *self);                                      // 0x0C
    bool            (*LoadBrandPS)(dscDESC_FAMILY *self, const dsTSTRING<char> *, psSECTION *); // 0x10
    dscBRAND        *(*CreateBrand)(dscDESC_FAMILY *self);                              // 0x14
} dscDESC_FAMILY_vtbl;

typedef struct dscDESC_FAMILY {
    dscDESC_FAMILY_vtbl *__vftable;      // 0x00
    bool                  isReplaceAllowed; // 0x04
    unsigned char         _pad05[3];        // 0x05
    // 0x08 — brand name -> pending config-file section, consumed (and erased) by
    // dscBRAND::Init as each brand parses its section.
    ds::MAP<dsTSTRING<char>, psSECTION, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> psList;
    dsVECTOR<dscBRAND *, 8>       brandList;    // 0x5C name-ascending sorted brand pointers
    dsVECTOR<dsTSTRING<char>, 8>  orderedList;  // 0x70 registration-order brand names
    dsTSTRING<char>               name;         // 0x84 family name
    bool                          isAllowReload; // 0x88

    // 0x827108E8 (const, QBA) — binary search brandList (name-ascending) for `nameBrand`;
    // returns the matching dscBRAND, or null if not registered.
    dscBRAND *FindBrand(const dsTSTRING<char> &nameBrand) const;

    // 0x8270F5C8 (static, SA) — walk `brand`'s parent chain, returning true if any ancestor's
    // name equals `nameParent` (byte-for-byte, via cached buffer-pointer identity or a length
    // + memcmp-style compare), false if the chain is exhausted (including brand having no
    // parent at all).
    static bool IsDerivedFrom(const dscBRAND *brand, const dsTSTRING<char> &nameParent);

    // Register a new dscBRAND for descriptor type DESC under `name` (parented under
    // `nameParent`), returning the created brand or null. One instantiation per registered
    // descriptor type; bodies not decompiled individually in this batch (boundary).
    template<class DESC> dscBRAND *RegisterBrand(const dsTSTRING<char> &name, const dsTSTRING<char> &nameParent);
} dscDESC_FAMILY;
