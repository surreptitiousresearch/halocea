/* createBreakableShapeFromBlueprint @0x8383A5F8 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShape.h"
#include "../headers/havok/hkdFracture.h"
#include "../headers/havok/FractureInfo.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/hkdGraphicsSystem.h"
#include "../headers/havok/hkdAssetProcessingUtil_GraphicsNode.h"

typedef struct hkpShape hkpShape;

/* Boundaries -- Havok SDK, not reversed. */
extern hkdBreakableShape *hkdAssetProcessingUtil_createBreakableShape(
    const char *name, const hkdGraphicsShape *graphicsShape, const hkpShape *physicsShape,
    hkdGraphicsSystem *graphicsSystem, unsigned char mergeCoplanarTriangles,
    unsigned char createGeometry); /* hkdAssetProcessingUtil::createBreakableShape */
extern void hkdBreakableShape_setGraphicsShape(hkdBreakableShape *self, const hkdGraphicsShape *graphicsShape); /* hkdBreakableShape::setGraphicsShape */
extern void hkdBreakableShape_setSimpleValuesFromBlueprint(hkdBreakableShape *self, const hkdShape *shapeBp); /* hkdBreakableShape::setSimpleValuesFromBlueprint */

/* Build an hkdBreakableShape for a graphics node from an optional shape
   blueprint. The blueprint (plus the fracture info) decides two flags handed to
   the SDK factory: whether coplanar triangles are merged, and whether geometry
   is created (createGeometry). createGeometry stays set unless a fracture that
   does not refit physics shapes -- or no static fracture at all -- makes it
   redundant, gated by isDeformable and the presence of a dynamic fracture. */
hkdBreakableShape *createBreakableShapeFromBlueprint(
    hkdShape *shapeBp,
    const hkpShape *physicsShape,
    FractureInfo *fractureInfo,
    hkdAssetProcessingUtil_GraphicsNode *graphicsNode,
    hkdGraphicsSystem *graphicsSystem,
    unsigned char isDeformable)
{
    hkdBreakableShape *breakableShape;
    unsigned char createGeometry = 1;
    unsigned char mergeCoplanarTriangles = 1;

    if (shapeBp)
    {
        hkdFracture *fracture = fractureInfo->m_fracture;
        /* m_refitPhysicsShapes is the hkEnum storage byte (decompile: .m_storage) */
        if (!fracture || !fracture->m_refitPhysicsShapes.m_storage)
            createGeometry = !isDeformable;
        if (!fracture)
            createGeometry = fractureInfo->m_dynamicFracture == 0 ? 0 : createGeometry;
        mergeCoplanarTriangles = shapeBp->m_mergeCoplanarTriangles.m_bool != 0;
    }
    else
    {
        createGeometry = 0;
    }

    /* m_graphics is hkRefPtr<hkdGraphicsShape>; pass its raw m_pntr */
    breakableShape = hkdAssetProcessingUtil_createBreakableShape(
        graphicsNode->m_name,
        graphicsNode->m_graphics.m_pntr,
        physicsShape,
        graphicsSystem,
        mergeCoplanarTriangles,
        createGeometry);
    if (breakableShape)
    {
        hkdBreakableShape_setGraphicsShape(breakableShape, graphicsNode->m_graphics.m_pntr);
        breakableShape->m_graphicsShapeName = graphicsNode->m_name;
        if (shapeBp)
            hkdBreakableShape_setSimpleValuesFromBlueprint(breakableShape, shapeBp);
    }
    return breakableShape;
}
