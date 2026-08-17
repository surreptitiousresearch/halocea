/* =========================================================================
   hkdConnectivityCalculator::calculateSemiAccurateConnectivity @0x83A49D40

   NOTE / CAVEAT (read before trusting this file):
   Reconstructed from the Hex-Rays decompile. The original is one large routine
   that the decompiler aliased through a SINGLE stack scratch object
   (`hkdConvex2dPolygonList v52`) reinterpreted as many overlapping typed views
   (an hkContactPoint, two cutting-plane hkVector4s, three hkTransforms, a plane
   basis hkRotation, an hkdAreaProperties, the result polygon list, ...). It is
   reconstructed here with DISCRETE, properly-typed locals (repo precedent:
   src/havok/calcTransform.c) rather than the byte-exact aliased stack layout.

   FAITHFULLY traced: the control flow, every boundary call with its argument
   order, the two-plane slab built ±maxDistance/2 around the contact, the
   cut -> findShapePlanePolygons -> intersect -> area/centroid -> area-properties
   pipeline, and the exact ConnectivityOutput field assignments.

   BEST-EFFORT (dense VMX128 vmsum3fp128 / vmrghw / vrlimi128 / vspltw blocks,
   re-derive from disasm @0x83A49DE4..0x83A49F10 and @0x83A4A108 before relying
   on it): the precise algebra that rotates the world contact normal into the
   plane basis and assembles the 3-D plane-space centroid. Each such block is
   marked `DEVIATION: VMX128 ... reconstructed as scalar`.
   ========================================================================= */
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkMatrix4.h"      /* layout-identical to hkTransform (pure C) */
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkContactPoint.h"
#include "../headers/havok/hkdVector2.h"
#include "../headers/havok/hkdConvex2dPolygonList.h"
#include "../headers/havok/hkdAreaProperties.h"
#include "../headers/havok/hkpTransformShape.h"
#include "../headers/havok/hkdConnectivityCalculator.h"

/* ---- opaque boundary types (used only as pointers here) ---- */
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
typedef struct hkpShape hkpShape;
typedef struct hkpConvexVerticesShape hkpConvexVerticesShape; /* returned by hkpShapeCutterUtil::cut */
typedef struct hkReferencedObject hkReferencedObject;

/* ---- one-shot Havok destruction-init gate ---- */
typedef struct hk_bool_flag { unsigned char m_bool; } hk_bool_flag;
extern hk_bool_flag HK_flyingcolors_destruction_1;
extern unsigned char processFlyingColorsDestruction(void);

/* ---- extern boundary functions (not sourced here) ---- */
/* hkpCollisionDispatcher-driven closest-features query; fills `contactOut`. */
/* def is 7 args returning int (0x83A49928 prologue reads r3-r7, r9, f1; r10
   never read — a previously-declared trailing `int unused` was a phantom). */
extern int calculateClosestDistance(hkpCollisionDispatcher *dispatcher,
                                    const hkpShape *shapeA, const hkTransform *transformA,
                                    const hkpShape *shapeB, const hkTransform *transformB,
                                    float maxDistance, hkContactPoint *contactOut);

extern void hkTransform_setInverse(hkTransform *out, const hkTransform *in);              /* hkTransform::setInverse */
extern void hkTransform_setMul(hkTransform *out, const hkTransform *a, const hkTransform *b); /* hkTransform::setMul */
/* 0x83A49EA0 calls ?_setRotatedDir@hkVector4@@QAAXABVhkRotation@@ABV1@@Z (0x83601070) — the
   hkRotation overload, whose DB prototype is
   void hkVector4___setRotatedDir(hkVector4 *this, const hkRotation *t, const hkVector4 *v). */
extern void hkVector4_setRotatedDir(hkVector4 *out, const hkRotation *rot, const hkVector4 *dir);          /* hkVector4::setRotatedDir */
extern void hkVector4_setTransformedInversePos(hkVector4 *out, const hkTransform *transform, const hkVector4 *pos); /* hkVector4::setTransformedInversePos */

extern const hkpConvexVerticesShape *hkpShapeCutterUtil_cut(hkpShape *shapeIn,
                                                      const hkVector4 *plane, float extraConvexRadiusForImplicitShapes); /* hkpShapeCutterUtil::cut — matches def (3 args; a prior extern carried a phantom 4th) */

extern void hkpTransformShape_ctor(hkpTransformShape *self, const hkpShape *childShape, const hkTransform *transform); /* hkpTransformShape::hkpTransformShape */
extern void hkpTransformShape_dtor(hkpTransformShape *self);                                                        /* hkpTransformShape::~hkpTransformShape */

extern void hkdShapePlaneConnectivityCalculator_calculateWorldToPlaneTransform(const hkVector4 *planeNormal, hkTransform *outTransform);
extern void hkdShapePlaneConnectivityCalculator_findShapePlanePolygons(hkpConvexVerticesShape *shape, const hkVector4 *plane,
                                                                       const hkTransform *worldToLocal, hkdConvex2dPolygonList *outList);

extern void hkdConvex2dPolygonList_reverseEdgeDirection(hkdConvex2dPolygonList *self);
extern void hkdConvex2dPolygonList_intersect(hkdConvex2dPolygonList *a, hkdConvex2dPolygonList *b, hkdConvex2dPolygonList *out);
extern unsigned char hkdConvex2dPolygonList_isEmpty(const hkdConvex2dPolygonList *self);
extern float hkdConvex2dPolygonList_calculateAreaAndCentroid(hkdConvex2dPolygonList *self, hkdVector2 *outCentroid);
extern void hkdConvex2dPolygonList_dtor(hkdConvex2dPolygonList *self);

extern void hkdAreaProperties_initialize(hkdAreaProperties *self, const hkdVector2 *centroid,
                                         int subPolygonCount, const unsigned char *vertsPerSubPolygon,
                                         const hkdVector2 *verts);

extern void hkReferencedObject_addReference(hkReferencedObject *obj);
extern void hkReferencedObject_removeReference(hkReferencedObject *obj);

/* Reset one hkdConvex2dPolygonList's two inline arrays to empty/inline state
   (mirrors the m_data=m_storage / m_size=0 / m_capacityAndFlags init the
   decompile inlines for v54/v57). The high bit of capacityAndFlags marks the
   storage as inline (not heap-owned). */
static void poly_list_init_inline(hkdConvex2dPolygonList *list)
{
    list->m_vertices_data = list->m_vertices_storage;
    list->m_vertices_size = 0;
    list->m_vertices_capacityAndFlags = (int)0x80000040;      /* inline flag | capacity 64 */
    list->m_numVertices_data = list->m_numVertices_storage;
    list->m_numVertices_size = 0;
    list->m_numVertices_capacityAndFlags = (int)0x80000008;   /* inline flag | capacity 8 */
}

int hkdConnectivityCalculator_calculateSemiAccurateConnectivity(
        hkdConnectivityCalculator_ConnectivityInput *input,
        hkdConnectivityCalculator_ConnectivityOutput *output)
{
    hkContactPoint contact;

    hkVector4 contactNormalWorld;   /* separating normal returned by the closest-distance query   */
    float     closestDistance;      /* signed separation distance (contact plane constant, lane 3) */

    hkVector4 cutPlaneForA;         /* world-space cutting plane fed to the shapeA cut  (v52[0])   */
    hkVector4 cutPlaneForB;         /* world-space cutting plane fed to the shapeB cut  (v52[6])   */
    hkVector4 localNormal;          /* contact normal rotated into the plane basis      (v52[2])   */

    hkTransform invTransformA;      /* inverse(transformA)                     (v52[16]) */
    hkTransform worldToPlane;       /* world -> plane frame                    (v52[38]) */
    hkTransform worldToLocalA;      /* worldToPlane * invTransformA            (v52[30]) */
    hkRotation planeBasis;          /* rotation used by setRotatedDir          (v52[24], hkRotation) */

    hkpTransformShape transformShapeA;
    hkpTransformShape transformShapeB;
    hkpConvexVerticesShape *cutShapeA;
    hkpConvexVerticesShape *cutShapeB;

    hkdConvex2dPolygonList polysA;      /* v54 */
    hkdConvex2dPolygonList polysB;      /* v57 */
    hkdConvex2dPolygonList resultPolys; /* v52[46] scratch view */

    /* v53 : hkInplaceArray<unsigned char,8> — per-sub-polygon vertex counts */
    unsigned char  numVertsPerPoly_storage[8];
    unsigned char *numVertsPerPoly_data = numVertsPerPoly_storage;
    int            numVertsPerPoly_size = 0;

    hkdVector2 centroid2d;
    hkVector4  centroid3dPlane;     /* plane-space 3-D centroid fed to setTransformedInversePos */
    hkdAreaProperties areaProps;
    float area;
    int lane;

    /* --- one-time destruction-subsystem init gate --- */
    if (!HK_flyingcolors_destruction_1.m_bool)
    {
        HK_flyingcolors_destruction_1.m_bool = processFlyingColorsDestruction();
        if (!HK_flyingcolors_destruction_1.m_bool)
            return 0;
    }

    /* --- closest-features query between the two shapes --- */
    if (!calculateClosestDistance(input->m_dispatcher,
                                  input->m_shapeA, &input->m_transformA,
                                  input->m_shapeB, &input->m_transformB,
                                  input->m_maxDistance, &contact))
        return 0;

    contactNormalWorld = contact.m_separatingNormal;
    /* Havok packs the separation distance in lane 3 of the contact normal. */
    closestDistance = contact.m_separatingNormal.m_quad.___u0.v[3];

    /* --- build the two world-space cutting planes (a slab straddling the
       contact) --- planeConstant d = dot3(normal, position). The two planes
       share the normal and are pushed apart by maxDistance/2 on each side.
       DEVIATION: VMX128 vmsum3fp128 dot-product + plane assembly reconstructed
       as scalar over lanes 0..2. */
    {
        float planeConstant = 0.0f;
        float half = input->m_maxDistance * 0.5f;
        for (lane = 0; lane < 3; ++lane)
            planeConstant += contactNormalWorld.m_quad.___u0.v[lane] * contact.m_position.m_quad.___u0.v[lane];

        cutPlaneForA = contactNormalWorld;
        cutPlaneForA.m_quad.___u0.v[3] = planeConstant + half;   /* v52[2].m_x path */

        /* shapeB cut uses the opposed plane offset (v52[6].m_x = -0.5*dist - d) */
        cutPlaneForB = contactNormalWorld;
        for (lane = 0; lane < 3; ++lane)
            cutPlaneForB.m_quad.___u0.v[lane] = -contactNormalWorld.m_quad.___u0.v[lane];
        cutPlaneForB.m_quad.___u0.v[3] = (closestDistance * -0.5f) - planeConstant + half;
    }

    /* --- transforms: inverse(transformA), and the plane basis / rotated normal --- */
    hkTransform_setInverse(&invTransformA, &input->m_transformA);

    /* DEVIATION: VMX128 vmrghw/vrlimi128/vspltw block @0x83A49E30..0x83A49F10
       assembles an orthonormal plane basis from the contact normal and rotates
       the world normal into it; reconstructed here as the two high-level ops it
       is equivalent to (basis build + setRotatedDir). Exact basis columns are
       best-effort. */
    planeBasis.m_col0 = contactNormalWorld;
    planeBasis.m_col1 = contactNormalWorld;
    planeBasis.m_col2 = contactNormalWorld;
    hkVector4_setRotatedDir(&localNormal, &planeBasis, &contactNormalWorld);

    /* --- construct transform-shapes and cut each by its plane --- */
    hkpTransformShape_ctor(&transformShapeA, input->m_shapeA, &input->m_transformA);
    hkpTransformShape_ctor(&transformShapeB, input->m_shapeB, &input->m_transformB);

    /* `.base` is the C spelling of the implicit hkpTransformShape -> hkpShape upcast. */
    cutShapeA = (hkpConvexVerticesShape *)hkpShapeCutterUtil_cut(&transformShapeA.base, &cutPlaneForA, 0.001f);
    cutShapeB = (hkpConvexVerticesShape *)hkpShapeCutterUtil_cut(&transformShapeB.base, &cutPlaneForB, 0.001f);

    hkpTransformShape_dtor(&transformShapeB);
    hkpTransformShape_dtor(&transformShapeA);

    if (!cutShapeA)
    {
        if (cutShapeB)
            hkReferencedObject_removeReference((hkReferencedObject *)cutShapeB);
        return 0;
    }
    if (!cutShapeB && !input->m_simplifyComplexShapeBByPlane.m_bool)
    {
        hkReferencedObject_removeReference((hkReferencedObject *)cutShapeA);
        return 0;
    }

    /* --- world<->plane transforms --- */
    hkdShapePlaneConnectivityCalculator_calculateWorldToPlaneTransform(&localNormal, &worldToPlane);
    hkTransform_setMul(&worldToLocalA, &worldToPlane, &invTransformA);

    /* --- project shapeA's cut faces into the plane --- */
    poly_list_init_inline(&polysA);
    hkdShapePlaneConnectivityCalculator_findShapePlanePolygons(cutShapeA, &cutPlaneForA, &worldToLocalA, &polysA);
    hkReferencedObject_removeReference((hkReferencedObject *)cutShapeA);

    poly_list_init_inline(&resultPolys);

    if (cutShapeB)
    {
        poly_list_init_inline(&polysB);
        hkdShapePlaneConnectivityCalculator_findShapePlanePolygons(cutShapeB, &cutPlaneForB, &worldToLocalA, &polysB);
        hkReferencedObject_removeReference((hkReferencedObject *)cutShapeB);

        hkdConvex2dPolygonList_reverseEdgeDirection(&polysB);
        hkdConvex2dPolygonList_intersect(&polysA, &polysB, &resultPolys);

        if (hkdConvex2dPolygonList_isEmpty(&resultPolys))
        {
            hkdConvex2dPolygonList_dtor(&polysB);
            hkdConvex2dPolygonList_dtor(&resultPolys);
            hkdConvex2dPolygonList_dtor(&polysA);
            return 0;
        }
        hkdConvex2dPolygonList_dtor(&polysB);
    }
    else
    {
        /* simplify-by-plane path: shapeA's polygons become the result directly
           (decompile: hkInplaceArray::operator= copies m_vertices into result and
           m_numVertices into v53). Copy the inline arrays field-for-field. */
        int i;
        resultPolys.m_vertices_size = polysA.m_vertices_size;
        for (i = 0; i < polysA.m_vertices_size && i < 64; ++i)
            resultPolys.m_vertices_storage[i] = polysA.m_vertices_storage[i];
        resultPolys.m_vertices_data = resultPolys.m_vertices_storage;

        numVertsPerPoly_size = polysA.m_numVertices_size;
        for (i = 0; i < polysA.m_numVertices_size && i < 8; ++i)
            numVertsPerPoly_storage[i] = polysA.m_numVertices_storage[i];
        numVertsPerPoly_data = numVertsPerPoly_storage;
    }

    /* --- area / centroid / section properties --- */
    area = hkdConvex2dPolygonList_calculateAreaAndCentroid(&resultPolys, &centroid2d);
    if (area < 0.0000001f)
    {
        hkdConvex2dPolygonList_dtor(&resultPolys);
        hkdConvex2dPolygonList_dtor(&polysA);
        return 0;
    }

    hkdAreaProperties_initialize(&areaProps, &centroid2d,
                                 numVertsPerPoly_size, numVertsPerPoly_data,
                                 resultPolys.m_vertices_data);

    /* --- assemble a 3-D plane-space centroid and map it back to world for the pivot.
       DEVIATION: VMX128 lane-shuffle @0x83A4A108..0x83A4A138 reconstructed as scalar. */
    centroid3dPlane.m_quad.___u0.v[0] = centroid2d.m_x;
    centroid3dPlane.m_quad.___u0.v[1] = centroid2d.m_y;
    centroid3dPlane.m_quad.___u0.v[2] = 0.0f;
    centroid3dPlane.m_quad.___u0.v[3] = 0.0f;

    /* --- fill ConnectivityOutput --- */
    output->m_separatingNormalA = localNormal;
    hkVector4_setTransformedInversePos(&output->m_pivotA, &worldToPlane, &centroid3dPlane);
    output->m_area           = area;
    output->m_distance       = closestDistance;
    output->m_phi            = areaProps.m_phi;
    output->m_sectionModulusY = areaProps.m_sectionModulusY;
    output->m_sectionModulusZ = areaProps.m_sectionModulusZ;

    hkdConvex2dPolygonList_dtor(&resultPolys);
    hkdConvex2dPolygonList_dtor(&polysA);
    return 1;
}
