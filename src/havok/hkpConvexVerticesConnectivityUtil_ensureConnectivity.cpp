/* ?ensureConnectivity@hkpConvexVerticesConnectivityUtil@@SA?AW4hkResult@@PBVhkpConvexVerticesShape@@@Z @0x83F938C0 */
#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkReferencedObject.h"

/* hkpConvexVerticesConnectivityUtil::ensureConnectivity
   If the shape has no connectivity object yet, compute one and attach it, then
   drop the local reference (setConnectivity takes its own). Returns 1 when the
   connectivity could not be computed, 0 otherwise. */
int hkpConvexVerticesConnectivityUtil_ensureConnectivity(hkpConvexVerticesShape *shape)
{
    const hkpConvexVerticesConnectivity *connectivity;

    if (!shape->m_connectivity)
    {
        connectivity = hkpConvexVerticesConnectivityUtil_findConnectivity(shape);
        if (!connectivity)
            return 1;
        hkpConvexVerticesShape_setConnectivity(shape, connectivity);
        /* DEVIATION: decompiler flattens the hkReferencedObject base to a direct
           cast; this struct models it as a named `base` member. */
        hkReferencedObject_removeReference((hkReferencedObject *)&connectivity->base);
    }
    return 0;
}
