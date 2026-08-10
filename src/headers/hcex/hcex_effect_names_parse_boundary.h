#pragma once
/* Boundary declarations for hcex_effect_names_parse.c.
 *
 * hcex_effect_names_parse loads "hcex_effect_names.ps" (a ws-engine property-sheet file) and populates
 * the sorted hcex_effect_names table (halo effect name -> hcex/ws effect name) from every top-level
 * section that carries both a "halo" and an "hcex" string key. The ps (property-sheet) system and its
 * iterator are ws-engine boundaries; their methods are declared below in the free-function
 * (compiler-generated-thiscall) form this function was reversed against. Reuses the
 * dsTSTRING<char> / dsVECTOR_PAIR / dsPAIR_TSTR types already declared for hcex_effect_names in
 * hcex_create_effect_boundary.h, to stay layout-consistent with that global; the ps TYPES
 * themselves come from their canonical ws/ps headers (see the note below). */

#include "hcex_create_effect_boundary.h"   /* dsTSTRING<char>, dsCMP, dsVECTOR_PAIR, dsPAIR_TSTR, hcex_effect_names,
                                               dsTSTRING_UnsafeInit, INS_DUP_IGNORE */

/* ps::SECTION — a 4-byte ref-counted section handle (canonical psSECTION; avoids C2011). */
#include "../ws/ps/psSECTION.h"

/* dsTSTRING<char>::UnsafeInitEmpty — adopt the shared empty-string singleton (boundary). */
extern void dsTSTRING_UnsafeInitEmpty(_Out_ dsTSTRING_flat *out); /* _Out_ per hcex_ds_boundary.h */

/* psSTD_TYPEID, psSECTION_KEY_REF and psITERATOR each used to have a SECOND body here, which is
 * `error: redefinition` in the header_layout probe TU and made the visible layout depend on
 * include order. All three now come from their canonical headers, reached through psITERATOR.h
 * (which pulls psSECTION_KEY_REF.h and ../ds/dsVECTOR.h). The bodies agreed member-for-member with
 * the canonicals — psSTD_TYPEID identically (including PS_TYPEID_SECTION = 7), psSECTION_KEY_REF
 * identically (ps@0, keyIdx@4, size 8), psITERATOR as a data-member subset of it (name@0, type@4,
 * flags@8, records@0xC, pos@0x20, size 36; the canonical adds the ctors/dtor/accessors).
 *
 * NOTE (provenance kept from the body deleted here): an earlier boundary spelling of psSTD_TYPEID
 * mis-set PS_TYPEID_SECTION to 2; the DB value is 7 — see the same note now in ws/ps/psITERATOR.h.
 * The consumer (hcex_effect_names_parse.cpp) keeps the free-function boundary spellings declared
 * below; the canonical psITERATOR additionally declares (never defines) a default ctor and dtor,
 * exactly the position psSECTION is already in in that same TU. */
#include "../ws/ps/psITERATOR.h"

typedef struct psSYSTEM psSYSTEM;
extern psSYSTEM *psSystem;

/* --- methods (compiler-generated-thiscall free-function form) --- */
extern void psSection_dtor(psSECTION *s);                                       /* psSECTION::~psSECTION */
extern void psSection_copy_ctor(psSECTION *dst, const psSECTION *src);          /* psSECTION::psSECTION(const psSECTION&) */

/* psSYSTEM::LoadPsFromFile(dsTSTRING<char> filename, dsTSTRING<char> subDir, dsTSTRING<char> ext, int flags)
 * — sret return: the parsed root section (empty on failure). */
extern psSECTION psSYSTEM_LoadPsFromFile(psSYSTEM *sys, dsTSTRING<char> filename, dsTSTRING<char> subDir,
        dsTSTRING<char> ext, int flags);

/* psITERATOR::psITERATOR(psSECTION root, psSTD_TYPEID filterType, unsigned long flags) */
extern void psITERATOR_ctor(psITERATOR *self, psSECTION root, psSTD_TYPEID filterType, unsigned long flags);
extern void psITERATOR_dtor(psITERATOR *self);
extern int  psITERATOR_IsDone(const psITERATOR *self);
extern void psITERATOR_Next(psITERATOR *self);
extern psSECTION psITERATOR_GetSection(const psITERATOR *self);   /* sret */

/* psSECTION_INTERFACE::GetDSString(const char *key, dsTSTRING<char> *out, unsigned long flags) const —
 * value under `key` coerced to a string into *out; returns whether it existed. `self` is any psSECTION
 * handle viewed through the interface (layout-identical: a single pData pointer). */
extern int psSECTION_INTERFACE_GetDSString(const psSECTION *self, const char *key, dsTSTRING<char> *out,
        unsigned long flags);

extern void dsVECTOR_PAIR_InsertSorted(dsVECTOR_PAIR *v, const dsPAIR_TSTR *entry, dsCMP *cmp, int dup_policy);
