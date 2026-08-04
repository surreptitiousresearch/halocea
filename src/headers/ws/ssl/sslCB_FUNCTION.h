#pragma once
#include "sslFUNCTION_BASE.h"
// ssl subsystem: a native (C++) callback bound as a script-class function element (the pCbFunc arm
// of sslCLASS_ELEMENT). DB-verified layout (types_members sslCB_FUNCTION): anonymous sslFUNCTION_BASE@0
// (12 bytes) + a callback-binding union@12 (size 16). The leading sslFUNCTION_BASE is modeled as public
// inheritance (promotes pDecl/pClassOfDefinition/stateFunc). stateFunc bit 0x2 = "callable while the
// owning object is still constructing"; bit 0x4 = the global (object-independent) binding form.

struct sslOBJECT;
struct sslCB_HOST;
struct sslOBJ_REF;
struct sslERROR;
struct dsDATA;

struct sslCB_FUNCTION : sslFUNCTION_BASE {
    // 0x0C callback binding (DB anonymous union _16F9F254B35DC6BCFC26D2FD9B5FA104, 4 bytes) — arms
    // injected into struct scope so bodies access pCbObject/pCbGlobal directly. Object form receives
    // the object's callback host as `this`; global form receives the object reference by value.
    union {
        void (*pCbObject)(sslCB_HOST *host, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
        void (*pCbGlobal)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
    };

    // 0x82AE0D68 — invoke the native callback on `pSelf` with (argc,argv)->retVal, on behalf of
    // `pCaller`; returns an sslERROR by value (sret).
    sslERROR Eval(sslOBJECT *pSelf, int argc, dsDATA *argv, dsDATA &retVal, sslOBJECT *pCaller);
};
