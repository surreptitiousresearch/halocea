#pragma once
#include "hkBool.h"
#include "hkTransform.h"
#include "hkpShape.h"

/* hkpTransformCollapseUtil helper types and entry points. Boundary -- Havok SDK,
   not decompiled. Struct layouts from DB types_members. */

/* The DB nests this enum inside hkpTransformCollapseUtil
   (hkpTransformCollapseUtil::SharedShapeBehaviour); scoped in a namespace here to keep
   the unqualified name collision-free across headers. */
namespace hkpTransformCollapseUtilEnums
{
    /* types_enum_values hkpTransformCollapseUtil::SharedShapeBehaviour */
    enum SharedShapeBehaviour
    {
        ALWAYS_COLLAPSE                 = 0,
        NEVER_COLLAPSE                  = 1,
        COLLAPSE_IF_LESS_THAN_THRESHOLD = 2,
    };
}

/* size 12 */
typedef struct hkpTransformCollapseUtil_Options
{
    hkpTransformCollapseUtilEnums::SharedShapeBehaviour m_sharedShapeBehaviour; /* 0 */
    int m_sharedShapeThreshold;      /* 4 */
    hkBool m_propageTransformInList; /* 8 */
} hkpTransformCollapseUtil_Options;

/* size 16 */
typedef struct hkpTransformCollapseUtil_Results
{
    int m_numCollapsedShapes;             /* 0  */
    int m_numIdentityTransformsRemoved;   /* 4  */
    int m_numSpecializedTransformShapes;  /* 8  */
    int m_numPropagatedTransformsToLists; /* 12 */
} hkpTransformCollapseUtil_Results;

void hkpTransformCollapseUtil_Options_construct(hkpTransformCollapseUtil_Options *self); /* Options::Options */
void hkpTransformCollapseUtil_Results_construct(hkpTransformCollapseUtil_Results *self); /* Results::Results */

/* Return the shape rebaked into the given transform (may return the input shape
   unchanged when the transform is identity). */
const hkpShape *hkpTransformCollapseUtil_transformTransformShape(const hkpShape *shape,
                                                                 const hkTransform *transform);

/* Collapse redundant transform shapes, writing the resulting shape to *collapsedOut. */
void hkpTransformCollapseUtil_collapseTransforms(const hkpShape *shape,
                                                 const hkpTransformCollapseUtil_Options *options,
                                                 hkpTransformCollapseUtil_Results *results,
                                                 const hkpShape **collapsedOut);
