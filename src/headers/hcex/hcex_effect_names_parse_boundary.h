#pragma once
/* Boundary declarations for hcex_effect_names_parse.c.
 *
 * hcex_effect_names_parse loads "hcex_effect_names.ps" (a ws-engine property-sheet file) and populates
 * the sorted hcex_effect_names table (halo effect name -> hcex/ws effect name) from every top-level
 * section that carries both a "halo" and an "hcex" string key. The ps (property-sheet) system and its
 * iterator are ws-engine boundaries; only the shape this function touches is modeled. Reuses the flat
 * dsTSTRING<char> / dsVECTOR_PAIR / dsPAIR_TSTR types already declared for hcex_effect_names in
 * hcex_create_effect_boundary.h (rather than the templated ws/ps headers) to stay layout-consistent
 * with that global. */

#include "hcex_create_effect_boundary.h"   /* dsTSTRING<char>, dsCMP, dsVECTOR_PAIR, dsPAIR_TSTR, hcex_effect_names,
                                               dsTSTRING_UnsafeInit, INS_DUP_IGNORE */

/* ps::SECTION — a 4-byte ref-counted section handle (canonical psSECTION; avoids C2011). */
#include "../ws/ps/psSECTION.h"

/* dsTSTRING<char>::UnsafeInitEmpty — adopt the shared empty-string singleton (boundary). */
extern void dsTSTRING_UnsafeInitEmpty(dsTSTRING_flat *out);

/* DB-verified enum (types_enum_values psSTD_TYPEID). NOTE: an earlier boundary spelling
 * mis-set PS_TYPEID_SECTION to 2; the DB value is 7 (the ctor call site references the symbol,
 * so this correction is transparent). */
typedef enum psSTD_TYPEID
{
    PS_TYPEID_INVALID = -1,
    PS_TYPEID_NULL    = 0,
    PS_TYPEID_INT     = 1,
    PS_TYPEID_FLOAT   = 2,
    PS_TYPEID_BOOL    = 3,
    PS_TYPEID_STRING  = 4,
    PS_TYPEID_OLD     = 5,
    PS_TYPEID_ARRAY   = 6,
    PS_TYPEID_SECTION = 7,
} psSTD_TYPEID;

/* psSECTION_KEY_REF — a section handle + key index (DB-verified, types_members) — size 8. */
typedef struct psSECTION_KEY_REF
{
    psSECTION ps;      /* 0x00 */
    int       keyIdx;  /* 0x04 */
} psSECTION_KEY_REF;

/* dsVECTOR<psSECTION_KEY_REF,8> (20B) — canonical template (avoids C2953 duplicate definition). */
#include "../ws/ds/dsVECTOR.h"

/* ps::ITERATOR — walks the (name,type)-filtered child records of a section.
 * DB-verified layout (types_members psITERATOR) — size 36. */
struct psITERATOR
{
    dsTSTRING<char>                          name;    /* 0x00 name filter */
    psSTD_TYPEID                        type;    /* 0x04 type filter */
    unsigned int                       flags;   /* 0x08 */
    dsVECTOR<psSECTION_KEY_REF, 8>     records; /* 0x0C matching key refs */
    int                                pos;     /* 0x20 current index */
};

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
