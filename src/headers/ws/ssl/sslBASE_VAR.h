#pragma once
// ssl subsystem: a script-class variable / constant slot — the `pVar` arm of sslCLASS_ELEMENT for
// the settable/gettable element kinds (element-type discriminators 3, 4 and, for reads, 5). Only the
// portion exercised by sslCLASS::Set/GetMemberValue is recovered.
// DB-verified layout (types_members sslBASE_VAR): __vftable@0 (sslBASE_VAR_vtbl*), pType@4,
// pClassOfDefinition@8 — size 12.

struct sslOBJECT;
struct sslERROR;
struct sslCLASS;
struct dsDATA;
struct dsDATA_TYPE;
struct sslBASE_VAR;

// DB types_members sslBASE_VAR_vtbl: dtor@0, Get@4, Set@8.
typedef struct sslBASE_VAR_vtbl {
    void (*dtr_sslBASE_VAR)(sslBASE_VAR *self, int deleteFlag);                                              // 0x00 — deleting dtor: vftable+0x00 holds ??_EsslBASE_VAR@@UAAPAXI@Z
    sslERROR *(*Get)(sslBASE_VAR *self, sslERROR *result, sslOBJECT *, int, dsDATA *);       // 0x04
    sslERROR *(*Set)(sslBASE_VAR *self, sslERROR *result, sslOBJECT *, int, const dsDATA *); // 0x08
} sslBASE_VAR_vtbl;

typedef struct sslBASE_VAR {
    sslBASE_VAR_vtbl  *__vftable;          // 0x00 virtual dispatch table
    const dsDATA_TYPE *pType;              // 0x04 declared value type (dsDATA_TYPE_STORAGE<NULL_TYPE> when untyped)
    sslCLASS          *pClassOfDefinition; // 0x08 owning class the slot was declared on

    // vtable slot 0x04 — read this member of `pSelf` into `out`; returns sslERROR by value (sret).
    sslERROR GetValue(sslOBJECT *pSelf, int idx, dsDATA &out); // boundary — virtual, not decompiled here
    // vtable slot 0x08 — store `val` into this member of `pSelf`; returns sslERROR by value (sret).
    sslERROR SetValue(sslOBJECT *pSelf, int idx, const dsDATA &val); // boundary — virtual, not decompiled here
} sslBASE_VAR;
