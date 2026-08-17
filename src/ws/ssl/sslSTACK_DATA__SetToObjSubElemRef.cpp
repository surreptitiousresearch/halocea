/* ?SetToObjSubElemRef@sslSTACK_DATA@@QAAAAV1@PAVsslOBJECT@@PAVsslCLASS@@H@Z @0x82AF0828 */
#include "sslSTACK_DATA.h"
#include "sslOBJECT.h"
#include "sslCLASS.h"

// Global atomic-increment helper; body not reversed here.
extern "C" long osLockedIncrement(volatile long *pValue); // boundary

// 0x82AF0828 — sslSTACK_DATA::SetToObjSubElemRef. Stores object+class+member-index into the
// subElemRef arm and adds a reference to each non-null of object and class.
sslSTACK_DATA &sslSTACK_DATA::SetToObjSubElemRef(sslOBJECT *obj, sslCLASS *cls, int idx)
{
    Cleanup();
    subElemRef.pObj = obj;
    subElemRef.pObjClass = cls;
    type = TYPE_SSLOBJECT_SUBELEM_REF;
    subElemRef.idx = idx;
    if (obj)
        osLockedIncrement((volatile long *)&obj->refCount.refCount);
    if (cls)
        osLockedIncrement((volatile long *)&cls->refCount.refCount);
    return *this;
}
