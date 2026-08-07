#pragma once
/* Boundary declarations for hcex_cine_init.c — one-time registration of the HCEX cinematic SSL script
 * callbacks (SetHaloCineScreenEffectActive, HcexStartCine, HcexDontSkipCine, AddCine,
 * HideCineActorsByTplName) on the global gsSslSystem's script class.
 *
 * DEVIATION (2026-08-07, odr_dup drain): this header used to restate sslOBJ_REF and sslCLASS_REF as
 * flat 4-byte plain-C structs, beside the canonical ws-engine definitions in src/headers/ws/ssl/ —
 * two `error: redefinition`s in header_layout's probe TU. Layouts were identical (DB types_members:
 * sslOBJ_REF pObject@0 size 4; sslCLASS_REF pClass@0 size 4), so this was rot rather than a layout
 * divergence, and the canonical bodies win by default. The flattened `(out, self, ...)` sret shims
 * that went with them are gone too, because the disassembly of hcex_cine_init @0x823B9C88 shows the
 * ordinary C++ method/RAII shape per registration block:
 *   bl sslSYSTEM::GetGlobalObj(void) -> bl sslOBJ_REF::GetClass(void)
 *   -> bl sslCLASS_REF::AddCbFunc(char const *,void (*)(sslOBJ_REF,int,dsDATA *,dsDATA &,sslOBJ_REF),char const *,int *)
 *   -> bl dlFree (the returned sslERROR's desc buffer) -> bl sslCLASS_REF::~sslCLASS_REF(void)
 *   -> bl sslOBJ_REF::~sslOBJ_REF(void)
 * i.e. destruction in reverse declaration order at each block's end and NO default-construction of
 * the two refs — which is what the sibling hcex_library_init.cpp @0x823D7280 (the identical
 * registration shape, already written against the canonical headers) has always spelled.
 *
 * Merged provenance / defect fixed in the same pass: the five callbacks were declared here as
 * `void (void)` and cast to `(void *)` at every call site, while their definitions
 * (src/hcex/cb*.cpp) are all `void (sslOBJ_REF, int, dsDATA *, dsDATA *, sslOBJ_REF)`. The real
 * signatures are declared below, so the casts are gone and the call sites are type-checked. */

#include "../ws/ssl/sslSYSTEM.h"    /* sslSYSTEM::GetGlobalObj() — returns sslOBJ_REF by value (sret) */
#include "../ws/ssl/sslOBJ_REF.h"   /* sslOBJ_REF::GetClass()    — returns sslCLASS_REF by value (sret) */
#include "../ws/ssl/sslCLASS_REF.h" /* sslCLASS_REF::AddCbFunc   — returns sslERROR by value (sret) */
#include "../ws/ssl/sslERROR.h"

extern sslSYSTEM *gsSslSystem;

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

/* SSL callback bodies (registered by name; external to this batch — declared only so their
 * addresses can be passed to AddCbFunc). */
extern "C" void cbSetHaloCineScreenEffectActive(sslOBJ_REF self, int argc, dsDATA *argv,
                                                dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbHcexStartCine(sslOBJ_REF self, int argc, dsDATA *argv,
                                dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbHcexDontSkipCine(sslOBJ_REF self, int argc, dsDATA *argv,
                                    dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbAddCine(sslOBJ_REF self, int argc, dsDATA *argv,
                          dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbHideActorsByTplName(sslOBJ_REF self, int argc, dsDATA *argv,
                                       dsDATA *retVal, sslOBJ_REF caller);
