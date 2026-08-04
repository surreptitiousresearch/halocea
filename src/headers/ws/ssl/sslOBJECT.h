#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsDATA.h"
#include "../ap/apSTATE_T.h"
#include "sslCB_HOST.h"
#include "sslCLASS_REF.h"
// ssl subsystem: a live script object instance. DB-verified layout (types_members sslOBJECT):
//   dsREF_COUNT@0, name@4, state@8, pCbHost@12, refClass@16, arrStoredMembers@20, dbgName@24,
//   uid@28 — size 32.

typedef struct sslOBJECT {
    dsREF_COUNT              refCount;         // 0x00
    dsTSTRING<char>          name;             // 0x04 object name
    apSTATE_T<unsigned long> state;            // 0x08 script state
    sslCB_HOST              *pCbHost;          // 0x0C callback host
    sslCLASS_REF             refClass;         // 0x10 class this object instantiates
    dsDATA                  *arrStoredMembers; // 0x14 stored member values
    dsTSTRING<char>          dbgName;          // 0x18 debug name
    int                      uid;              // 0x1C unique object id

    // 0x82A37928 — the callback host bound to this object.
    sslCB_HOST *GetCbHost();
    // 0x825246E0 — copy of the debug name (returns a fresh dsTSTRING<char> by value). const (QBA).
    dsTSTRING<char> GetDbgName() const;

    // 0x82A37E28 — tear down: decrement the live-object counter and notify the owning system
    // (_OnObjectDelete), destroy every stored member value (each dsDATA's runtime-type Destroy,
    // in reverse index order, matching the compiler's eh-vector-destructor-iterator over the
    // arrStoredMembers array) and free the array, release dbgName/name buffers, and release refClass.
    ~sslOBJECT();
} sslOBJECT;

// Process-wide count of live sslOBJECT instances, decremented in the destructor (DB symbol __sslNObj).
extern int __sslNObj;
