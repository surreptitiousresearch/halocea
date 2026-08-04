#pragma once
/* Shared boundary declarations for the ws-engine data-structure (ds*) primitives used by the HCEX bridge
 * helpers in this batch (hcex_add_model_node, hcex_print_compass, hcex_fire_event).
 *
 * dsTSTRING_flat / dsVECTOR / dsSTRID / dsEVENT_MGR are ws-engine (Saber) C++ template/utility types treated as
 * extern boundaries per the project's re-source conventions: only the fields the bridge actually touches are
 * modeled, at their database offsets, and the C++ "methods" are declared as free functions whose first
 * parameter is the object (mirroring the compiler-generated thiscall the decompiler shows). */

/* --- ref-counted string: canonical dsTSTRING<char>, aliased to the boundary convention's
 * dsTSTRING_flat name (identical layout: pBuffer@0, 4 bytes). --- */
#include "../ws/ds/dsTSTRING.h"

typedef dsTSTRING<char> dsTSTRING_flat;

/* --- interned string id (dsSTRID) — a single pointer, passed by value. Use the canonical
 * header (identical layout: id@0) to avoid a C2011 redefinition when both are pulled into a TU. --- */
#include "../ws/ds/dsSTRID.h"

/* --- event manager (opaque) --- */
typedef struct dsEVENT_MGR dsEVENT_MGR;

extern dsEVENT_MGR *gEventMgr;

/* --- ws container / utility methods (free-function form of the C++ thiscall) --- */
extern void       dsTSTRING_UnsafeInit(dsTSTRING_flat *s, const char *src, int len, int flags);
extern void       dsTSTRING_UnsafeInitEmpty(dsTSTRING_flat *s); /* dsTSTRING<char>::UnsafeInitEmpty() -- adopt the
                                                             * process-wide shared empty-string singleton
                                                             * (lazily allocated on first use via a function-
                                                             * local static guard inside UnsafeInitEmpty itself;
                                                             * not re-derived at each call site). */
extern void       dsVECTOR_TSTRING_PushBack(void *vector, const dsTSTRING_flat *value); /* dsVECTOR<dsTSTRING<char>,8>::PushBack */
extern dsTSTRING_flat *dsSPrintf(dsTSTRING_flat *result, const char *fmt, ...);
extern void       dsSTRID_ctor(dsSTRID *s, const char *name, bool existOnly);      /* dsSTRID::dsSTRID(const char*, bool) */
extern int        dsEVENT_MGR_RegisterEvent(dsEVENT_MGR *mgr, dsSTRID event);
extern void       dsEVENT_MGR_SignalEvent(dsEVENT_MGR *mgr, int event, const void *params);
extern void       dsTSTRING_Clear(dsTSTRING_flat *s);   /* dsTSTRING<char>::Clear(void) — reset to empty */

/* --- allocator --- */
extern "C" void dlFree(void *ptr); // C linkage: dlFree is an unmangled C export; matches canonical decls
