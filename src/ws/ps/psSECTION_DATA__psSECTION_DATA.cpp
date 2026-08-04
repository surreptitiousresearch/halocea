#include "psSECTION_DATA.h"
#include <cstddef>
#include "psSECTION_BRANCH.h"
#include "../ds/dsSTRID.h"
#include "../../headers/apCL.h"
// 0x825177AC — default-construct an empty section body: interned-empty name, no owner/parents/
// records, and a freshly allocated owning branch (refCount 0, pTopSection = this, isNeedRescan set).
// `__nPsData` is a global live-count of allocated psSECTION_DATA bodies, bumped here.

// 0x8200155A — interned empty-string literal used to build the default (unnamed) section name.
extern const char psEmptyStringLiteral[];
extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug allocator
extern int __nPsData; // boundary — global live psSECTION_DATA count

psSECTION_DATA::psSECTION_DATA()
    : refCount(1), name(psEmptyStringLiteral, 1), pOwner(nullptr), state(0)
{
    apCL cl;
    cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp";
    cl.line = 1509;
    psParents.pData = nullptr;
    psParents.nElem = 0;
    psParents.allocated = 0;
    psParents.__cl = cl;

    cl.line = 1510;
    orderedRecords.pData = nullptr;
    orderedRecords.nElem = 0;
    orderedRecords.allocated = 0;
    orderedRecords.__cl = cl;

    psSECTION_BRANCH *branch = static_cast<psSECTION_BRANCH *>(operator new(
        sizeof(psSECTION_BRANCH), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 0x5ECu));
    if (branch) {
        branch->refCount = 0;
        branch->pTopSection = this;
        branch->isDeleting = 0;
        branch->isNeedRescan = 1;
    }
    pBranch = branch;

    ++__nPsData;
}
