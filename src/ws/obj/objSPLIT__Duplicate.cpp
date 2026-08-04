#include <new>
#include "objSPLIT.h"
#include "../ds/dsVECTOR.h"
#include "obj_boundary.h"
// 0x82BCAD98  ?Duplicate@objSPLIT@@QAAPAV1@XZ
// Allocate a fresh objSPLIT and deep-copy this one into it: size the entry array, then copy-construct
// each entry (which allocates a private spuConfig + material vector) and copy the source SPU config
// block into it. Returns nullptr on allocation failure.
//
// CAVEAT: the original copies the objSPU_CONFIG payload by open-coding a 10-machine-word walk via
// spuConfig[-1].pUserData with pre-increment; that region equals the whole 40-byte objSPU_CONFIG, so
// it is reproduced here as a 10-word copy of *spuConfig.

// Debug-tagged global operator new(size, file, line) — boundary (not reversed here).
extern void *operator new(size_t size, const char *file, unsigned int line);

objSPLIT *objSPLIT::Duplicate()
{
    static const char *const kFile = "D:\\Projects\\code\\common\\src.sys\\objects\\obj_split.cpp";

    objSPLIT *dup = (objSPLIT *)operator new(sizeof(objSPLIT), kFile, 0x29B);
    if (!dup)
        return nullptr;

    dup->nEntry = 0;
    dup->entryList = nullptr;
    if (!dup->Alloc(this->nEntry))
        return nullptr;

    for (int i = 0; i < this->nEntry; ++i) {
        objSPLIT_ENTRY *srcEntry = &this->entryList[i];
        objSPLIT_ENTRY *dupEntry = &dup->entryList[i];
        if (dupEntry != srcEntry) {
            dlFreeAligned(dupEntry->spuConfig);
            dupEntry->spuConfig = nullptr;
            dupEntry->mtlTbl.mtlList.~dsVECTOR();        // release the Alloc-initialized material vector
            new (dupEntry) objSPLIT_ENTRY(*srcEntry);    // copy-construct (allocates a private spuConfig)
        }

        if (this->entryList[i].spuConfig) {
            // STRONG_ASSERT: dup->entryList[i].spuConfig != 0  (obj_split.cpp:677)
            void **srcWords = (void **)this->entryList[i].spuConfig;
            void **dupWords = (void **)dup->entryList[i].spuConfig;
            for (int w = 0; w < 10; ++w)  // 10 words == full 40-byte objSPU_CONFIG (see CAVEAT)
                dupWords[w] = srcWords[w];
        }
    }

    return dup;
}
