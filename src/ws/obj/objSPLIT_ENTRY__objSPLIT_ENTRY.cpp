#include <stdint.h>
#include "objSPLIT.h"
#include <string.h>

extern "C" void *dlMemalign(uint32_t size, uint32_t align, const char *file, uint32_t line); // boundary

// objSPLIT_ENTRY::objSPLIT_ENTRY(const objSPLIT_ENTRY &src) @ 0x8268A600
// Copy-construct one render-batch entry from another: deep-copy the material table, copy the
// vertex/face range and bone info by value, and (if present) allocate a fresh 40-byte SPU config
// block and duplicate its contents.
// CAVEAT: objSPU_CONFIG is an opaque boundary type (no DB struct layout resolved) — its size
// (0x28 = 40 bytes) is known only from the dlMemalign call site here, and the decompile's
// pointer-walk copy (10 words, offset by one element due to a `[-1].pUserData` base) is a
// decompiler artifact of a straight 40-byte blob copy; reproduced as memcpy of that size.
// NOTE: the decompile also shows `this->maxPSLength = p->maxVSLength;` (reads the SOURCE's
// maxVSLength for the destination's maxPSLength) — kept as-is; matches the disassembly, so this
// looks like a latent bug in the original game code rather than a decompiler misread.
objSPLIT_ENTRY::objSPLIT_ENTRY(const objSPLIT_ENTRY &src) :
    mtlTbl(src.mtlTbl)
{
    this->startVert = src.startVert;
    this->nVert = src.nVert;
    this->startFace = src.startFace;
    this->nFace = src.nFace;
    this->spuConfig = nullptr;
    this->bonesInfo = src.bonesInfo;
    this->skinCompoundId = src.skinCompoundId;
    this->maxVSLength = src.maxVSLength;
    this->maxPSLength = src.maxVSLength; // see NOTE above — matches source, not a typo introduced here

    if (src.spuConfig) {
        this->spuConfig = (objSPU_CONFIG *)dlMemalign(0x28u, 0x10u,
            "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 0x1C2u);
        memcpy(this->spuConfig, src.spuConfig, 0x28u);
    }
}
