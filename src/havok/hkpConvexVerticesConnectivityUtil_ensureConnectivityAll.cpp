/* ?ensureConnectivityAll@hkpConvexVerticesConnectivityUtil@@SA?AW4hkResult@@PBVhkpShape@@@Z @0x83F93930 */
#include "../headers/havok/hkpConvexVerticesShape.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpShapeType.h"
#include "../headers/havok/hkResult.h"
#include "../headers/havok/hkReferencedObject.h"

/* Boundary callees (2-level descent). For a shape collection the children are
   walked through the container sub-object that begins right after the convex
   base (offset 16); getFirstKey/getChildShape/getNextKey are its virtuals.
   DEVIATION: the decompiler reads these as raw vtable slots off &shape->m_radius;
   modelled here as an opaque container pointer + named accessors. */
typedef struct hkpShapeContainer hkpShapeContainer;
extern int hkpShapeContainer_getFirstKey(const hkpShapeContainer *container);                                   /* virtual */
extern const hkpShape *hkpShapeContainer_getChildShape(const hkpShapeContainer *container, int key, void *buffer);/* virtual */
extern int hkpShapeContainer_getNextKey(const hkpShapeContainer *container, int key);                            /* virtual */

/* hkpConvexVerticesConnectivityUtil::ensureConnectivityAll — make sure every
   convex-vertices leaf reachable from shapeIn owns a connectivity object,
   generating and attaching one where it is missing. Recurses through shape
   collections. Returns HK_SUCCESS (0) if all leaves are consistent, HK_FAILURE
   (1) if connectivity could not be produced for one of them. */
int hkpConvexVerticesConnectivityUtil_ensureConnectivityAll(hkpConvexVerticesShape *shapeIn)
{
    unsigned int shapeType = shapeIn->m_type.m_storage; /* m_type is on hkpShape base (inherited), not hkReferencedObject */

    if (shapeType == HK_SHAPE_CONVEX_VERTICES)
    {
        if (!shapeIn->m_connectivity)
        {
            const hkpConvexVerticesConnectivity *connectivity =
                hkpConvexVerticesConnectivityUtil_findConnectivity(shapeIn);
            if (!connectivity)
                return HK_FAILURE;
            hkpConvexVerticesShape_setConnectivity(shapeIn, connectivity);
            hkReferencedObject_removeReference((hkReferencedObject *)&connectivity->base);
        }
    }
    else if (shapeType == HK_SHAPE_COLLECTION)
    {
        const hkpShapeContainer *container = (const hkpShapeContainer *)((char *)shapeIn + 16);
        int key = hkpShapeContainer_getFirstKey(container);
        if (key != -1)
        {
            unsigned char childBuffer[520];
            while (1)
            {
                const hkpShape *child = hkpShapeContainer_getChildShape(container, key, childBuffer);
                if (hkpConvexVerticesConnectivityUtil_ensureConnectivityAll((hkpConvexVerticesShape *)child) == HK_FAILURE)
                    return HK_FAILURE;
                key = hkpShapeContainer_getNextKey(container, key);
                if (key == -1)
                    return HK_SUCCESS;
            }
        }
    }

    return HK_SUCCESS;
}
