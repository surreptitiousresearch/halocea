#pragma once
#include "sslSCRIPTED_FUNCTION_BASE.h"
#include "sslERROR.h"
// ssl subsystem: a fully compiled script function. DB-verified layout (types_members
// sslSCRIPTED_FUNCTION): anonymous sslSCRIPTED_FUNCTION_BASE@0 — size 96 (no additional members).
// Modeled as public inheritance so the base fields (stateFunc, localVars, ...) are promoted.

struct sslOBJECT; // ssl script object instance (full definition in sslOBJECT.h)
struct dsDATA;    // ds type-erased value (full definition in ds/dsDATA.h)

struct sslSCRIPTED_FUNCTION : sslSCRIPTED_FUNCTION_BASE {
    // 0x82ADF2A8 — evaluate this compiled function on `pSelf` for `pCaller`, writing the result into
    // `*retVal`; returns sslERROR by value (sret). Converts params, logs, runs the bytecode, coerces
    // the return value to the declared type, and restores the stack.
    sslERROR Eval(sslOBJECT *pSelf, int argc, dsDATA *retVal, sslOBJECT *pCaller);
};
