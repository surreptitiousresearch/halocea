#pragma once
/* Boundary declarations for the ws-engine debug-variable system (dbgVAR*) touched by the HCEX bridge:
 * the HCEX debug-console bool toggles, plus the dbgVAR_MANAGER lookup/set path used to record the
 * named-checkpoint string. Layout verified via types_members: dbgVAR is 12 bytes (vtable/name/typeId),
 * and each bool toggle is a dbgVAR_SIMPLE<bool,1> = dbgVAR + value/prevValue/defaultValue. The manager
 * methods are declared as free functions (compiler-generated thiscall form). */

#include "hcex_ds_boundary.h"   /* dsTSTRING<char> */
/* Use the single canonical dbgVAR definition (ws/wb/dbgVAR_boundary.h) instead of redefining it
 * locally — avoids C2011 when a TU pulls both headers. Layout is identical. */
#include "../ws/wb/dbgVAR_boundary.h"   /* dbgVAR, dbgVAR_vtbl, dbgVAR_SIMPLE<> */

/* dbgVAR_bool / dbgVAR_float / dbgVAR_STRING are the plain-named spellings of the DB template
 * instantiations dbgVAR_SIMPLE<bool,1> / <float,3> / <dsTSTRING<char>,4>. Expressed by inheriting
 * the canonical dbgVAR_IMPL<T,N> (single dbgVAR definition; consumers use `.value`, inherited). */
struct dbgVAR_bool   : dbgVAR_IMPL<bool, 1>            {}; /* value@0x0C — 16 bytes */
struct dbgVAR_float  : dbgVAR_IMPL<float, 3>           {}; /* value@0x0C — 24 bytes */

/* DEVIATION (2026-08-07, odr_dup drain): dbgVAR_STRING used to be spelled here as a third
 * `: dbgVAR_IMPL<dsTSTRING<char>,4> {}` line, duplicating the body in ws/dbg/dbgVAR_STRING.h —
 * one `error: redefinition` in header_layout's probe TU. The canonical body wins on DB evidence:
 * types_members dbgVAR_STRING is a single ANONYMOUS BASE-CLASS member of type
 * dbgVAR_SIMPLE<dsTSTRING<char>,4> at offset 0 (size 24), i.e. the SIMPLE level, not the IMPL
 * level this copy inherited from. Layout is identical either way (dbgVAR_SIMPLE<T,N> adds no
 * fields over dbgVAR_IMPL<T,N> — it exists to install its own vtable), and no consumer here
 * touches a field: hcex_get_autosave_name.cpp goes through dbgVAR_STRING_CStr() below. */
#include "../ws/dbg/dbgVAR_STRING.h"

typedef struct dbgVAR_MANAGER dbgVAR_MANAGER;

extern dbgVAR_MANAGER *dbgVAR_MANAGER_GetManager(void);
extern dbgVAR *dbgVAR_MANAGER_FindFirst(dbgVAR_MANAGER *mgr, const dsTSTRING<char> *name);
extern void dbgVAR_MANAGER_SetDbgVarValue_tstring(dbgVAR_MANAGER *mgr, dbgVAR *var, const dsTSTRING<char> *value); /* SetDbgVarValue<dsTSTRING<char>,4> */
extern void dbgVAR_MANAGER_SetDbgVar_tstring(dbgVAR_MANAGER *mgr, const dsTSTRING<char> *name, const dsTSTRING<char> *value); /* SetDbgVar<dsTSTRING<char>,4> (set-by-name) */

/* dbgVAR_STRING accessor (ws-engine boundary). */
extern const char *dbgVAR_STRING_CStr(const dbgVAR_STRING *self);

/* HCEX debug-console string var holding the checkpoint base name. */
extern "C" dbgVAR_STRING dbg_hcex_ChptName;

/* HCEX debug-console toggles (dbgVAR_SIMPLE<bool,1>). */
extern "C" dbgVAR_bool dbg_hcex_GenChptNamed;
extern "C" dbgVAR_bool dbg_hcex_DisableCine;
extern "C" dbgVAR_bool dbg_hcex_DumpCine;
extern "C" dbgVAR_bool dbg_hcex_CineMakeSave;
extern "C" dbgVAR_bool dbg_hcex_DumpCineExtras;   /* extra per-frame cine dump logs (camera time, sound) */
extern "C" dbgVAR_bool dbg_hcex_HideCineActors;   /* hide cinematic actors while their cine clip is active */

/* HCEX debug-console float knob (dbgVAR_SIMPLE<float,3>). */
extern "C" dbgVAR_float dbg_hcex_inp_vibration_scale;
