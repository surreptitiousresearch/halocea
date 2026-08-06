#pragma once
/* Boundary declarations for hcex_cine_init.c — one-time registration of the HCEX cinematic SSL script
 * callbacks (SetHaloCineScreenEffectActive, HcexStartCine, HcexDontSkipCine, AddCine,
 * HideCineActorsByTplName) on the global gsSslSystem's script class. The ssl subsystem
 * (sslSYSTEM/sslOBJ_REF/sslCLASS_REF/sslERROR) is ws-engine C++ (real definitions in
 * src/headers/ws/ssl/*.h); only the flattened free-function form used by this bridge is declared here,
 * following the by-value-return-as-sret convention used throughout that subsystem (GetGlobalObj/GetClass
 * both return their result by value in the real headers). */

#include "../ws/ssl/sslERROR.h" /* sslERROR / sslERROR_ID — real ws-engine definition */

typedef struct sslSYSTEM  sslSYSTEM;
typedef struct sslOBJECT  sslOBJECT;
typedef struct sslCLASS   sslCLASS;

typedef struct sslOBJ_REF   { sslOBJECT *pObject; } sslOBJ_REF;     /* 4 bytes */
typedef struct sslCLASS_REF { sslCLASS  *pClass;  } sslCLASS_REF;   /* 4 bytes */

extern sslSYSTEM *gsSslSystem;

/* sslSYSTEM::GetGlobalObj() const — returns the system's global script object, by value (sret). */
extern void sslSYSTEM_GetGlobalObj(sslOBJ_REF *out, sslSYSTEM *self);
/* sslOBJ_REF::GetClass() const — the class of the referenced object, by value (sret). */
extern void sslOBJ_REF_GetClass(sslCLASS_REF *out, sslOBJ_REF *self);
/* sslOBJ_REF::~sslOBJ_REF() — release the referenced object, freeing it at zero refcount. */
extern void sslOBJ_REF_dtor(sslOBJ_REF *self);
/* sslCLASS_REF::~sslCLASS_REF() — release the referenced class, freeing it at zero refcount. */
extern void sslCLASS_REF_dtor(sslCLASS_REF *self);

/* sslCLASS_REF::AddCbFunc(const char *sig, CB fn, sslOBJ_REF hostObj, int *outExtra) — register a
 * native callback under the given SSL function signature string; returns an sslERROR by value (sret).
 * `cbFunc` is void(sslOBJ_REF, int, dsDATA*, sslERROR&, sslOBJ_REF); `hostObj`/`outExtra` are passed
 * through unused by every call site in this batch (host = the shared empty-string sentinel, extra =
 * null). */
extern void sslCLASS_REF_AddCbFunc(sslERROR *outErr, sslCLASS_REF *self, const char *sig, void *cbFunc,
                                    const void *hostObj, int *outExtra);

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

/* SSL callback bodies (registered by name; external to this batch — declared only so their
 * addresses can be passed to AddCbFunc). */
extern "C" void cbSetHaloCineScreenEffectActive(void);
extern "C" void cbHcexStartCine(void);
extern "C" void cbHcexDontSkipCine(void);
extern "C" void cbAddCine(void);
extern "C" void cbHideActorsByTplName(void);
