#include "../headers/havok/hkdWoodFracture_SplitParams.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkTransform.h"

extern const hkTransform hkTransformIdentity;
extern void hkArrayUtil__reserveMore(void *array, int elementSize);                                     /* hkArrayUtil::_reserveMore */
extern void hkdBreakableShape_addChild(hkdBreakableShape *self, const hkdBreakableShape *child,
                                       const hkTransform *transform);                                  /* hkdBreakableShape::addChild */

/* Append a freshly-created breakable child shape to the split-shapes output array
   with an identity transform and default runtime info, then (unless it is the root
   shape) register it as a child of the parent breakable shape. Part of the wood-
   fracture destruction path. Reconstructed from disasm 0x8384E740: the decompiler
   packed the 8-byte RuntimeInfo across a vector/GPR slot and mis-rendered the
   distance field as "m_size + 1"; disasm shows it is the FLT_MAX-like sentinel
   0x7F7FFFEE stored via stfs, so it is decoded here to the real field writes. */
void addNewShapeToParentAndRuntime(const hkdWoodFracture_SplitParams *splitParams, hkdBreakableShape *shape)
{
    hkArray_hkdShapeInstanceInfo *splitShapesOut = splitParams->m_splitShapesOut;

    if (splitShapesOut)
    {
        hkdShapeInstanceInfo *newInstance;
        int oldSize;

        if (splitShapesOut->m_size == (splitShapesOut->m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(splitShapesOut, sizeof(hkdShapeInstanceInfo));

        oldSize = splitShapesOut->m_size;
        splitShapesOut->m_size = oldSize + 1;
        newInstance = &splitShapesOut->m_data[oldSize];

        newInstance->m_transform = hkTransformIdentity;
        newInstance->m_shape = shape;
        newInstance->m_damage = 0;
        newInstance->m_runtimeInfo.m_distanceToDestructionPoint = 3.4028162e38f; /* 0x7F7FFFEE */
        newInstance->m_runtimeInfo.m_oldChildIdx = 0xFFFF;
        newInstance->m_runtimeInfo.m_forceFixed = 0;
    }

    if (shape != splitParams->m_rootBreakableShape)
    {
        if (splitParams->m_addNewShapesToParents.m_bool)
            hkdBreakableShape_addChild((hkdBreakableShape *)splitParams->m_rootBreakableShape, shape,
                                       &hkTransformIdentity);
    }
}
