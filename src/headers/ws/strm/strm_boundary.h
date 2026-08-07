#pragma once
#include "../ap/apCONFIG.h"
#include "../ap/apCOUNTER.h"
#include "../ds/ds_boundary.h"
#include "../ds/ds_assert_boundary.h"
#include "../ps/psSECTION.h"
#include "../ps/psARRAY.h"
#include "strmRESOURCE_TEXTURE_XENON.h"
#include "../wb/dbgVAR_boundary.h" // dbgVAR_SIMPLE<> template home (dbgVAR_TRIGGER base below)
// Shared boundary declarations for globals/helpers referenced by the strm* .cpp files in this
// folder but not themselves part of the strm subsystem.

extern apCONFIG *gCfg; // hcex bridge global — the live config database (see haloENGINE_CONTROL_boundary.h)

// dbgVAR_TRIGGER has a single definition in ws/dbg/dbgVAR_TRIGGER.h (DB: one anonymous
// dbgVAR_SIMPLE<bool,5> base at offset 0, 16 bytes; the trigger's bool payload lives at base
// offset 0x0C, the dbgVAR_IMPL layer). Include it rather than redefining locally, so the
// header_layout probe TU doesn't hit error: redefinition.
#include "../dbg/dbgVAR_TRIGGER.h"
extern "C" dbgVAR_TRIGGER d2; // debug gate for strmRES_TEX_MANAGER::UpdateResource2's tex_stat_na.csv dump

extern "C" apCOUNTER cnt_strm_mem; // "streaming memory" perf counter (unit index 2 = "not enough mem" MB)

// Most-recently-checked texture, cached by strmRES_TEX_MANAGER::_DbgCheckTex for its next call
// (a single static strmRESOURCE_TEXTURE_XENON, reused as scratch — matches the disasm's `prev`
// global symbol and the strmRESOURCE_TEXTURE_XENON::operator= call at the end of _DbgCheckTex).
extern "C" strmRESOURCE_TEXTURE_XENON prev;

// Formatted engine log (varargs) / forced-log-to-file variant. boundary.
void _apLog(const char *fmt, ...);
void _apForceLog(const char *fileName, const char *fmt, ...);

// Xbox 360 XDK graphics API (XGRAPHICS) — queries the base/mip region layout of a swizzled GPU
// texture. boundary — stock SDK function (outside this re-source). Parameter list/order (r3..r10) verified
// against the call site's disassembly (strmRES_TEX_MANAGER::_DbgCheckTex) rather than trusted
// from the decompiler's pseudocode, which mis-attributed 3 extra locals as extra arguments.
extern "C" void XGGetTextureLayout(const void *pTexture, void *pBaseData, unsigned int *pBaseSize,
                                    void *pBaseRegionList, unsigned int *pBaseRegionListCount,
                                    unsigned int baseRegionAlignment, void *pMipData,
                                    unsigned int *pMipSize);

// PowerPC intrinsic (count leading zeros, 32-bit) — Xbox 360 XDK ppcintrinsics.h names this
// _CountLeadingZeros; the disassembly emits the `cntlzw` instruction directly. boundary.
extern "C" unsigned int __cntlzw(unsigned int value);

// os thread/signal primitives used by strmSTREAMING_QUEUE (see os_boundary.h / fio_boundary.h for
// the rest of the os* surface already declared elsewhere).
struct osHANDLE_DUMMY;
extern "C" {
    osHANDLE_DUMMY *osCreateThread(void (*proc)(void *arg), void *arg, const char *name,
                                    int stackSize); // boundary
    void osSetThreadPriority(osHANDLE_DUMMY *thread, int priority); // boundary
    void osSetSignal(osHANDLE_DUMMY *signal);    // boundary
    void osResetSignal(osHANDLE_DUMMY *signal);  // boundary
    void osSleep(unsigned int milliseconds);      // boundary
}

// The binary ALSO carries the template instantiation
// ??$osCreateThread@VstrmSTREAMING_QUEUE@@@@YAPAUosHANDLE_DUMMY@@P6AXPAVstrmSTREAMING_QUEUE@@@Z0PBDH@Z
// = osCreateThread<T>(void (*)(T *), T *, const char *, int). Only the erased `void *` overload above
// was modelled, so strmSTREAMING_QUEUE::CreateThread had to reinterpret_cast its thread proc — a cast
// the original source never wrote. C++ linkage by construction, so it cannot sit in the block above.
template <class T>
osHANDLE_DUMMY *osCreateThread(void (*proc)(T *), T *arg, const char *name, int stackSize);
