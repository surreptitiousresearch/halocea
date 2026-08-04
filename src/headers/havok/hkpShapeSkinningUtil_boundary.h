#pragma once
#include "hkTransform.h"
#include "hkVector4.h"

typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
typedef struct hkpShape hkpShape;
typedef struct hkMeshShape hkMeshShape;

/* hkpShapeSkinningUtil::Input, size 36 */
typedef struct hkpShapeSkinningUtil_Input
{
    float m_maxInside;                             /* 0  */
    float m_maxOutside;                            /* 4  */
    hkpCollisionDispatcher *m_collisionDispatcher; /* 8  */
    const hkpShape **m_shapes;                     /* 12 */
    const hkTransform *m_transforms;               /* 16 */
    int m_numBones;                                /* 20 */
    int m_bonesPerVertex;                          /* 24 */
    hkVector4 *m_vertexPositions;                  /* 28 */
    int m_numVertices;                             /* 32 */
} hkpShapeSkinningUtil_Input;

void hkpShapeSkinningUtil_setSkinningValues(const hkpShapeSkinningUtil_Input *input, hkMeshShape *mesh); /* hkpShapeSkinningUtil::setSkinningValues */
