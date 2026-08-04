#include "sslSTACK_DATA.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"

// Global atomic-increment helper; body not reversed here.
extern "C" long osLockedIncrement(volatile long *pValue); // boundary

// 0x82AF07D0 — sslSTACK_DATA::SetToObjRef. Stores object+class into the objRef arm and adds a
// reference to each non-null side.
sslSTACK_DATA &sslSTACK_DATA::SetToObjRef(sslOBJECT *obj, sslCLASS *cls)
{
    Cleanup();
    objRef.pObj = obj;
    objRef.pClass = cls;
    type = TYPE_SSLOBJECT_REF;
    if (obj)
        osLockedIncrement((volatile long *)&obj->refCount.refCount);
    if (cls)
        osLockedIncrement((volatile long *)&cls->refCount.refCount);
    return *this;
}
