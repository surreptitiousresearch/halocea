#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
#include "../ps/psSECTION.h"
// boundary — dsc (descriptor system) base descriptor class. Reversed here to the extent the ds
// smart-pointer needs (vtable deleting-destructor slot, intrusive ref-count) plus ProcessPS.
// DB-verified layout (types_members dscDESC): __vftable@0, dsREF_COUNT@4, pBrand@8.

struct dscBRAND;
struct dscTYPE_INFO;
struct dscDESC;

// DB-verified (types_members dscDESC_vtbl). Only the first two slots are used here; the rest
// are declared for completeness. ParsePS/PostProcessPS mangle confirms `psSECTION` BY VALUE
// (not a pointer) — grep 'ParsePS'/'PostProcessPS' shows `?ParsePS@dscDESC@@MAAXVpsSECTION@@
// ABV?$dsTSTRING@D@@@Z`; the `V` before psSECTION is by-value. psSECTION's non-trivial copy
// ctor/dtor make this an "invisible reference" argument in this ABI (caller builds a temporary,
// passes its address, callee owns and destroys it) — matches the disasm at 0x82663198 which
// copy-constructs a fresh local from `psRead` for the call.
typedef struct dscDESC_vtbl {
    const dscTYPE_INFO *(*GetCurTypeInfo)(dscDESC *self);  // 0x00
    void                (*dtr_dscDESC)(dscDESC *self, int deleteFlag); // 0x04 (scalar-deleting dtr; deleteFlag bit0 => free)
    void                (*NotifyTermBrand)(dscDESC *self);  // 0x08
    void                (*ParsePS)(dscDESC *self, psSECTION section, const dsTSTRING<char> &hintErr);       // 0x0C
    void                (*PostProcessPS)(dscDESC *self, psSECTION section, const dsTSTRING<char> &hintErr); // 0x10
    void                (*RegisterSslFunctions)(dscDESC *self, struct sslCLASS_REF *sslClass);              // 0x14
} dscDESC_vtbl;

typedef struct dscDESC {
    dscDESC_vtbl *__vftable; // 0x00
    dsREF_COUNT   ref;       // 0x04 (DB: unnamed embedded dsREF_COUNT)
    dscBRAND     *pBrand;    // 0x08

    // 0x82663198 — dispatch a config-file section through this descriptor's virtual two-phase
    // parse pipeline: ParsePS then PostProcessPS. `psRead` is BY VALUE throughout (see
    // dscDESC_vtbl note above); each virtual call implicitly copy-constructs its own temporary
    // from it, matching the two separate psSECTION copy-constructions seen in the disassembly.
    void ProcessPS(psSECTION psRead, const dsTSTRING<char> &hintErr);
} dscDESC;
