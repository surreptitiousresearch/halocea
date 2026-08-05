#pragma once
#include <stdint.h>
#include "../ds/dsVECTOR.h"
#include "../mtl/mtlTBL.h"
// ws-engine obj: geometry "split" — the list of render batches (material + vertex/face range)
// a model is divided into. DB-verified layouts (types_members objSPLIT / objSPLIT_ENTRY).
// mtlTBL is the ws/mtl subsystem's fully-typed version (canonical — see ../mtl/mtlTBL.h);
// this header used to carry its own partial duplicate (texDensity as an opaque blob) before
// the mtl subsystem was reversed — consolidated here to avoid a duplicate-definition conflict
// when both subsystems are included in the same translation unit.

struct objSPU_CONFIG;      // boundary — pointer only
struct objSPLIT;           // self-reference

// Per-split bone range info. DB-verified layout (types_members objBONES_INFO) — size 6.
typedef struct objBONES_INFO {
    uint16_t start1; // 0x00
    uint16_t start2; // 0x02
    uint8_t  num1;   // 0x04
    uint8_t  num2;   // 0x05
} objBONES_INFO;

// One render batch of a split. DB-verified layout (types_members objSPLIT_ENTRY) — size 92.
typedef struct objSPLIT_ENTRY {
    mtlTBL           mtlTbl;         // 0x00 materials for this batch
    uint16_t __dummy__;      // 0x3C
    uint16_t startVert;      // 0x3E
    uint16_t nVert;          // 0x40
    uint16_t startFace;      // 0x42
    uint16_t nFace;          // 0x44
    unsigned char _pad0[2]; /* db-verified padding */
    objSPU_CONFIG   *spuConfig;      // 0x48
    objBONES_INFO    bonesInfo;      // 0x4C
    int16_t          skinCompoundId; // 0x52
    int              maxVSLength;    // 0x54
    int              maxPSLength;    // 0x58

    objSPLIT_ENTRY();                          // boundary — default construct
    objSPLIT_ENTRY(const objSPLIT_ENTRY &src); // boundary — copy construct
} objSPLIT_ENTRY;

// DB-verified layout (types_members objSPLIT) — size 8 (nEntry@0, entryList@4; 2 bytes pad).
typedef struct objSPLIT {
    uint16_t  nEntry;    // 0x00 number of live entries
    unsigned char _pad0[2]; /* db-verified padding */
    objSPLIT_ENTRY   *entryList; // 0x04 array of split entries

    // 0x82681828 — bounds-checked access to entry `i`.
    objSPLIT_ENTRY *GetEntry(int i);

    // 0x82BC9E20 — (re)size the entry array to `nEntry` entries, resetting shrunk/grown slots.
    // Returns 1 on success (also when nEntry==0), 0 on allocation failure.
    int Alloc(int nEntry);

    // 0x82BCAD98 — allocate a fresh split and deep-copy this one into it (used when unsharing
    // geometry). Returns nullptr on allocation failure.
    objSPLIT *Duplicate();

    // Destroy every entry (releasing each entry's material table). Body not decompiled in this
    // batch; invoked via `delete pSplit` from objOBJ::_DestroyData. boundary.
    ~objSPLIT();
} objSPLIT;
