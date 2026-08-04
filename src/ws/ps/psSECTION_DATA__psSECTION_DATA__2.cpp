#include "psSECTION_DATA.h"
#include <cstddef>
#include <new> // placement new (subCopy) psSECTION_DATA(...)
#include "psSECTION_BRANCH.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../ds/ds_boundary.h"
#include "../../headers/apCL.h"
// 0x8251F480 — deep-copy construct: same interned name, a fresh owning branch (own top-level
// refcount, not shared with `data`'s branch), a referenced copy of the parent list, and a
// record-by-record copy of the ordered records — sub-section-valued records are recursively
// deep-copied (and re-owned by this new body); all other records are copied via dsDATA::StoreValue.

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug allocator

psSECTION_DATA::psSECTION_DATA(const psSECTION_DATA &data)
    : refCount(1), pOwner(nullptr), state(data.state)
{
    name.id = data.name.id;

    apCL cl;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp";
    cl.line = 2172;
    psParents.pData = nullptr;
    psParents.nElem = 0;
    psParents.allocated = 0;
    psParents.__cl = cl;
    orderedRecords.pData = nullptr;
    orderedRecords.nElem = 0;
    orderedRecords.allocated = 0;
    orderedRecords.__cl = cl;

    psSECTION_BRANCH *branch = static_cast<psSECTION_BRANCH *>(operator new(
        sizeof(psSECTION_BRANCH), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 0x882u));
    if (branch) {
        branch->refCount = 0;
        branch->pTopSection = this;
        branch->isDeleting = 0;
        branch->isNeedRescan = 1;
    }
    pBranch = branch;

    psParents = data.psParents;
    orderedRecords.Resize(data.orderedRecords.nElem);

    for (int i = 0; i < data.orderedRecords.nElem; ++i) {
        // GetSubSection() is declared non-const (it does not mutate `data`, matching its existing
        // reversed body in psSECTION_DATA__GetSubSection.cpp); const_cast bridges that mismatch,
        // matching the mangled `const psSECTION_DATA&` copy-ctor parameter observed in the DB.
        psSECTION_DATA *subSection = const_cast<psSECTION_DATA &>(data).GetSubSection(i);
        psSECTION_RECORD &dst = orderedRecords[i];
        const psSECTION_RECORD &src = data.orderedRecords[i];

        if (subSection) {
            psSECTION_DATA *subCopy = static_cast<psSECTION_DATA *>(operator new(
                sizeof(psSECTION_DATA), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 0x88Bu));
            if (subCopy)
                new (subCopy) psSECTION_DATA(*subSection);
            dst.keyStrId.id = src.keyStrId.id;
            // Directly adopt `subCopy` as a psSECTION-typed value (no extra refcount bump: subCopy
            // was just freshly constructed with refCount==1, exactly matching a fresh handle).
            dst.value.type = dsDATA_TYPE_STORAGE<psSECTION>::dataType;
            reinterpret_cast<psSECTION *>(&dst.value.storage)->pData = subCopy;
            dst.sortedIdx = src.sortedIdx;
            dst.lineNmb = src.lineNmb;
            if (subCopy)
                subCopy->SetOwner(this);
        } else {
            dst.keyStrId.id = src.keyStrId.id;
            dst.value.type = nullptr;
            dst.value.StoreValue(src.value);
            dst.sortedIdx = src.sortedIdx;
            dst.lineNmb = src.lineNmb;
        }
    }
}
