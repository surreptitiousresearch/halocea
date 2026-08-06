#pragma once
#include "../../ws/ssl/sslCLASS_REF.h"
#include "../../ws/ssl/sslERROR.h"
#include "../../ws/ssl/sslOBJ_REF.h"
#include "../../ws/ds/dsDATA.h"

/* PerformExport() calls two sslCLASS_REF methods (AddCbFunc, RegisterEvent) that this SSL_EXPORTER
 * batch does not own the header for (sslCLASS_REF.h currently only models the ref-counted-pointer
 * core). DB-verified prototypes (funcs table), declared here as boundary free functions in
 * thiscall form — this corpus's established convention for extending a shared boundary type
 * without touching its owning header (see hcex_ds_boundary.h's dsTSTRING_UnsafeInit etc.). */

namespace sslCLASS_REF_bridge {

// 0x82525230 — register `decl` as a callable SSL function on `this_`'s class, bound to the
// (self,argc,argv,retVal,caller) callback `cbFunc`; writes the resolved function id through `idx`.
// Returns an sslERROR by value (sret, via `result`): no-error on success.
sslERROR *AddCbFunc(
    sslCLASS_REF *this_,
    sslERROR *result,
    const char *decl,
    void (*cbFunc)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
    const char *errHint,
    int *idx);

// 0x82525690 — register `decl` as a bare SSL event (no callback) on `this_`'s class, tagged with
// `mask`; writes the resolved event id through `idx`. Returns an sslERROR by value (sret, via
// `result`): no-error on success.
sslERROR *RegisterEvent(
    sslCLASS_REF *this_,
    sslERROR *result,
    const char *decl,
    unsigned int mask,
    const char *errHint,
    int *idx);

} // namespace sslCLASS_REF_bridge

// Saber logging sink (?_apLog@@YAXPBDZZ) — declared identically to this corpus's other
// _apLog boundary declarations (e.g. src/headers/ws/anim/anim_boundary.h). boundary.
void _apLog(const char *format, ...);
