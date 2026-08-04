#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkThreadMemory.h"
#include "../headers/havok/hkpShape.h"

#define HK_REAL_MAX 3.402823466e38f

/* Boundary types/callees (2-level descent limit). */
typedef struct hkpListShape hkpListShape;
extern int hkpListShape_getNumChildShapes(const hkpListShape *listShape);            /* hkpShapeContainer::getNumChildShapes */
extern const hkpShape *hkpListShape_getChildShape(hkpListShape *listShape, unsigned int key, char (*buffer)[512]); /* listShape->m_childInfo[key].m_shape */
extern void hkdBreakableShape_construct(hkdBreakableShape *self, const hkpShape *physicsShape,
                                        const void *parentName, const hkdGeometry *geometry);
extern const int HK_MEMORY_CLASS_DESTRUCTION;
extern const hkTransform hkTransformIdentity;
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* hkdAssetProcessingUtil::createShapeInstanceInfosFromPhysicsShape — wrap each
   child shape of a physics list shape in a freshly-constructed hkdBreakableShape
   and emit one identity-placed hkdShapeInstanceInfo per child into shapeInfos. */
void hkdAssetProcessingUtil_createShapeInstanceInfosFromPhysicsShape(const hkpListShape *listShape,
                                                                     hkArray<hkdShapeInstanceInfo> *shapeInfos)
{
    int numChildren = hkpListShape_getNumChildShapes(listShape);
    int capacity = shapeInfos->m_capacityAndFlags & 0x3FFFFFFF;
    int i = 0;
    char getChildScratch[512]; /* scratch buffer required by hkpListShape::getChildShape */

    if (capacity < numChildren)
    {
        int grow = 2 * capacity;
        if (numChildren >= grow)
            grow = numChildren;
        hkArrayUtil__reserve(&shapeInfos->m_data, grow, (int)sizeof(hkdShapeInstanceInfo));
    }
    shapeInfos->m_size = numChildren;

    for (i = 0; i < hkpListShape_getNumChildShapes(listShape); ++i)
    {
        const hkpShape *childShape = hkpListShape_getChildShape((hkpListShape *)listShape, i, &getChildScratch);
        hkdBreakableShape *breakable = (hkdBreakableShape *)hkThreadMemory_allocateChunk(
            hkThreadMemory_getCurrent(), 208, HK_MEMORY_CLASS_DESTRUCTION);
        hkdShapeInstanceInfo *info;
        ((hkReferencedObject *)breakable)->m_memSizeAndFlags = 208;
        hkdBreakableShape_construct(breakable, childShape, 0, 0);

        info = &shapeInfos->m_data[i];
        info->m_transform = hkTransformIdentity;
        info->m_shape = breakable;
        info->m_damage = 0;
        info->m_runtimeInfo.m_distanceToDestructionPoint = HK_REAL_MAX;
        info->m_runtimeInfo.m_oldChildIdx = 0xFFFF;
        info->m_runtimeInfo.m_forceFixed = 0;
    }
}
