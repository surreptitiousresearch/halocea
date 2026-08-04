#include "../headers/havok/hkdDecorateFractureFaceAction.h"
#include "../headers/havok/hkdDecorateFractureFaceActionInternalInfo.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdBreakableShape_Connection.h"
#include "../headers/havok/hkdGraphicsShape.h"
#include "../headers/havok/BodyInfo.h"
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpCollidable.h"
#include "../headers/havok/hkpCollisionDispatcher.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/hkAabb.h"
#include "../headers/math_constants.h"

#ifndef NULL
#define NULL nullptr
#endif

/*
 * CAVEAT — SIMD / register-packing reconstruction.
 *
 * decorateConnection is a leaf of the destruction pipeline compiled almost
 * entirely from VMX128 intrinsics; the decompiler emitted raw `__asm` blocks
 * (lvx128 / vmsum3fp128 / vpermwi128 / vrsqrtefp / vmaddcfp128 …) rather than C.
 * The geometry has been reconstructed to named, readable hkVector4 operations
 * (dot3 / cross / normalize / scale / madd).  The high-level algorithm and the
 * exact control flow are faithful; the permute-level bit fidelity of the dense
 * fused-multiply-add chains is best-effort.
 *
 * Two VMX128 physical registers were reused for values of different types (the
 * classic OVERLAPPED-packing case): fp31 held both the in-plane right axis (a
 * vector) and later a reciprocal span (a scalar); fp1/fp2 held both an in-plane
 * axis and later a getMaximumProjection result.  These have been split into
 * separate, descriptively-named variables.
 *
 * The true signature is 6 arguments: the decompiler's trailing float params
 * (arg seven onward) are phantoms produced by the PPC float-slot ABI.  The call sites
 * (0x83858998 / 0x838589D4) pass only r3..r7 and f1 (objectDir).  Verified
 * against the caller disassembly.
 */

/* ---- Havok SDK boundaries (not reversed) ------------------------------- */

/* hkVector4 SIMD helpers (hkVector4::* inline methods). */
extern void hkVector4_setRotatedDir(hkVector4 *dst, const hkRotation *rot, const hkVector4 *dir);   /* hkVector4::setRotatedDir */
extern void hkVector4_setTransformedPos(hkVector4 *dst, const hkTransform *t, const hkVector4 *p);  /* hkVector4::setTransformedPos */
extern void hkVector4_setTransformedInversePos(hkVector4 *dst, const hkTransform *t, const hkVector4 *p); /* hkVector4::setTransformedInversePos */
extern float hkVector4_dot3fpu(const hkVector4 *a, const hkVector4 *b);                             /* hkVector4::dot3fpu (?dot3fpu@hkVector4@@QBAMABV1@@Z) — scalar-return 3-lane dot; reconstruction of the inlined VMX vmsum3fp128 */
extern void hkVector4_setCross(hkVector4 *dst, const hkVector4 *a, const hkVector4 *b);             /* hkVector4::setCross */
extern void hkVector4_normalize3(hkVector4 *v);                                                     /* hkVector4::normalize<3> */
extern void hkVector4_setScaled(hkVector4 *dst, const hkVector4 *v, float s);                       /* hkVector4::setMul */
extern void hkVector4_setSub(hkVector4 *dst, const hkVector4 *a, const hkVector4 *b);               /* hkVector4::setSub */
extern void hkVector4_addScaledDir(hkVector4 *dst, const hkVector4 *dir, float s);                  /* hkVector4::addMul */
extern void hkVector4_setNeg4(hkVector4 *dst, const hkVector4 *v);                                  /* hkVector4::setNeg<4> */
extern int  hkVector4_allComponentsWithin(const hkVector4 *a, const hkVector4 *b, float epsilon);   /* per-component |a-b|<=eps compare */

/* hkpShape virtual dispatch (called through m_physicsShape vtable).
   DEVIATION: prior externs dropped the hkBool sret slot (castRay) and the hkAabb
   out-parameter (getAabb); the binary virtual calls pass r3=&sret hkBool,
   r4=this, r5=input, r6=output (@0x83857ABC) and r3=this, r4=localToWorld,
   f1=tolerance, r6=&aabbOut (@0x83857DF8). Declared to match the sourced
   forwarders in hkpShape_castRay.cpp / hkpShape_getAabb.cpp. */
struct hkpShapeRayCastInput;
struct hkpShapeRayCastOutput;
extern hkBool *hkpShape_castRay(hkpShape *self, hkBool *result,
                                const hkpShapeRayCastInput *input,
                                hkpShapeRayCastOutput *output);                                     /* hkpShape::castRay */
extern "C" void hkpShape_getAabb(hkpShape *self, const hkTransform *localToWorld,
                                 float tolerance, hkAabb *out);                                     /* hkpShape::getAabb */
extern float hkpShape_getMaximumProjection(const hkpShape *shape, const hkVector4 *direction);      /* hkpShape::getMaximumProjection (virtual) */

/* hkpCollidable construction + array growth. */
extern void hkpCollidable_ctor(hkpCollidable *self, const hkpShape *shape, const hkTransform *t, char type); /* hkpCollidable::hkpCollidable — 4th arg is the broadphase-handle type (binary passes an int in r6; prior extern spelled it void*) */
extern void hkArrayUtil__reserveMore(void *array, int elemSize);                                     /* hkArrayUtil::_reserveMore */

extern double floor(double x);

/* Collector vtables constructed on the stack for the linear casts. */
extern void *hkpClosestCdPointCollector_vftable;                                                    /* hkpClosestCdPointCollector::`vftable' */
extern void *hkpCdPointCollector_vftable;                                                           /* hkpCdPointCollector::`vftable' */

/* Ray-cast output scratch (hkpShapeRayCastOutput, 64 bytes); m_hitFraction lives
   at +16 and is the field the caller reads back. */
typedef struct RayCastOutput
{
    hkVector4 m_normal;      /* 0  */
    float m_hitFraction;     /* 16 */
    int m_extraInfo;         /* 20 */
    unsigned char reserved_24[40];
} RayCastOutput;

/* On-stack closest-point collector: vtable + closest distance + hit flag. */
typedef struct DecorationPointCollector
{
    void *m_vftable;            /* 0   */
    float m_closestDistance;    /* 4   */
    unsigned char reserved_8[36];
    float m_earlyOutDistance;   /* [sp+1DCh] guard used before a hit */
    int m_hasHit;               /* [sp+1E0h] nonzero once a contact is recorded */
} DecorationPointCollector;

#define HK_DEG_TO_RAD (DEG_TO_RAD)
/* LCG (Numerical-Recipes constants) used to jitter decoration placement. */
#define LCG_MUL 1664525u
#define LCG_ADD 1013904223u
#define LCG_TO_UNIT_FLOAT (2.3283064e-10f) /* 1 / 2^32 */

/*
 * decorateConnection — scatter decoration graphics shapes across one fracture
 * face of a breakable body.
 *
 * For the connection between two breakable pieces, build the world-space
 * separating plane (oriented by objectDir so the A and B faces point opposite
 * ways), deduplicate it against the planes already decorated for this
 * body/shape-instance, then classify the face as top / bottom / left / right by
 * its slope relative to the action's up direction.  The matching FractureFaceInfo
 * supplies the decoration set, density and grid spacing.  Candidate decoration
 * shapes are drawn at random, ray-cast against the piece to find the face
 * surface, snapped to the configured grid, gap-tested via a linear cast against
 * the previously placed decoration, and — if the density test passes — appended
 * to biA->m_graphics for later instancing.
 */
void decorateConnection(
    hkdDecorateFractureFaceAction *action,
    hkdDecorateFractureFaceActionInternalInfo *info,
    const hkdShapeInstanceInfo *sii,
    const hkdBreakableShape_Connection *connection,
    BodyInfo *biA,
    float objectDir)
{
    hkVector4 separatingPlane;   /* oriented world normal in xyz, plane offset in w */
    hkVector4 worldPivot;        /* connection pivot in world space */
    float planeOffset;           /* -(orientedNormal . worldPivot) */

    hkVector4 planeRightAxis;    /* in-plane horizontal axis = normalize(up x front) */
    hkVector4 planeUpAxis;       /* in-plane tangent axis (front projected into plane) */
    hkVector4 planeForwardAxis;  /* third frame axis */
    float tangentLengthSq;       /* |front projected into plane|^2 */
    float faceSlopeDot;          /* dot(planeForwardAxis, upDir); also L/R sign */

    hkVector4 segmentStart;      /* world-space probe/cast segment endpoints */
    hkVector4 segmentEnd;
    hkVector4 rayFromLocal;      /* segment transformed into shape-local space */
    hkVector4 rayToLocal;

    RayCastOutput rayOutput;
    hkBool rayHit;               /* castRay sret hit flag (binary r3=&var_417); fraction is what's consumed */
    float rayHitFraction;        /* mirrors rayOutput.m_hitFraction; <1 on a hit */
    int rayShapeKey0;
    int rayShapeKey1;
    int rayShapeKeyIndex;

    hkdDecorateFractureFaceAction_FractureFaceInfo *faceInfo;       /* chosen L/R/T/B face */
    hkInplaceArrayAligned16<hkdShapeInstanceInfo, 16> *decorationList; /* chosen decoration list */

    int usedNormalCount;
    int normalIndex;
    BodyInfo_Normal *usedNormals;

    hkVector4 castDirection;     /* linear-cast path (planeRightAxis - planeForwardAxis) */

    hkTransform decorationTransform;         /* candidate placement transform */
    hkTransform previousCollidableTransform;
    hkpCollidable fixedCollidable;
    hkpCollidable castCollidable;
    DecorationPointCollector pointCollector;

    const hkdBreakableShape *previousDecorationShape;
    unsigned int randomState;
    int randomSeed;

    hkVector4 forwardProbe;      /* byref direction for getMaximumProjection */
    hkVector4 negatedRight;

    /* --- world-space separating plane, oriented by objectDir --------------- */
    hkVector4_setRotatedDir(&separatingPlane, &sii->m_transform.m_rotation, &connection->m_separatingNormal);
    hkVector4_setTransformedPos(&worldPivot, &sii->m_transform, &connection->m_pivotA);
    hkVector4_setScaled(&separatingPlane, &separatingPlane, objectDir);
    planeOffset = -hkVector4_dot3fpu(&separatingPlane, &worldPivot);
    separatingPlane.m_quad.___u0.v[3] = planeOffset; /* pack plane offset into w */

    /* --- dedup against planes already decorated for this shape instance ---- */
    usedNormalCount = biA->m_usedNormals.m_size;
    if (usedNormalCount > 0)
    {
        usedNormals = biA->m_usedNormals.m_data;
        normalIndex = 0;
        while (1)
        {
            if (usedNormals[normalIndex].m_shapeInstance == sii)
            {
                /* plane already recorded within 0.02 tolerance -> nothing to do */
                if (hkVector4_allComponentsWithin(&usedNormals[normalIndex].m_normal,
                                                  &separatingPlane, 0.02f))
                    return;
            }
            if (++normalIndex >= usedNormalCount)
                break;
        }
    }

    /* --- record the new plane ---------------------------------------------- */
    if (biA->m_usedNormals.m_size == (biA->m_usedNormals.m_capacityAndFlags & 0x3FFFFFFF))
        hkArrayUtil__reserveMore(&biA->m_usedNormals, 32);
    {
        int newNormalIndex = biA->m_usedNormals.m_size;
        biA->m_usedNormals.m_size = newNormalIndex + 1;
        biA->m_usedNormals.m_data[newNormalIndex].m_normal = separatingPlane;
        biA->m_usedNormals.m_data[newNormalIndex].m_shapeInstance = sii;
    }

    /* --- build the in-plane orthonormal frame ------------------------------ */
    /* horizontal axis = normalize(up x front); tangent = front projected into
       the plane (front - normal*(normal.front)). */
    hkVector4_setCross(&planeRightAxis, &action->m_upDir, &action->m_frontDir);
    {
        float normalDotFront = hkVector4_dot3fpu(&separatingPlane, &action->m_frontDir);
        hkVector4 scaledNormal;
        hkVector4_setScaled(&scaledNormal, &separatingPlane, normalDotFront);
        hkVector4_setSub(&planeUpAxis, &action->m_frontDir, &scaledNormal);
    }
    tangentLengthSq = hkVector4_dot3fpu(&planeUpAxis, &planeUpAxis);
    hkVector4_normalize3(&planeRightAxis);

    if (tangentLengthSq == 0.0f)
        return; /* degenerate tangent — nothing to decorate */

    {
        float invTangentLength = 1.0f / tangentLengthSq;

        /* complete the frame and build a probe segment through the pivot that
           spans the face along the tangent, offset by the plane's fixed limits. */
        hkVector4_setCross(&planeForwardAxis, &planeUpAxis, &planeRightAxis);
        hkVector4_setScaled(&segmentStart, &planeUpAxis, invTangentLength);
        segmentEnd = worldPivot;
        hkVector4_addScaledDir(&segmentEnd, &planeUpAxis, -100.0f); /* _real_c2c80000 */
        hkVector4_addScaledDir(&segmentStart, &planeForwardAxis, 0.009962f); /* _real_bc23d70a */

        /* first probe: locate the face surface along the tangent */
        hkVector4_setTransformedInversePos(&rayFromLocal, &sii->m_transform, &segmentStart);
        hkVector4_setTransformedInversePos(&rayToLocal, &sii->m_transform, &segmentEnd);
        rayOutput.m_hitFraction = 1.0f;
        rayHitFraction = 1.0f;
        rayShapeKey0 = -1;
        rayShapeKey1 = -1;
        rayShapeKeyIndex = 0;
        hkpShape_castRay((hkpShape *)sii->m_shape->m_physicsShape.m_pntr, &rayHit,
                         (const hkpShapeRayCastInput *)&rayFromLocal, (hkpShapeRayCastOutput *)&rayOutput);
        rayHitFraction = rayOutput.m_hitFraction;
        if (rayHitFraction >= 1.0f)
            return;

        /* advance to the hit and re-probe from there */
        hkVector4_addScaledDir(&segmentStart, &planeUpAxis, 100.0f);        /* _real_42c80000 */
        hkVector4_addScaledDir(&segmentStart, &planeForwardAxis, 0.009962f);/* _real_3c23d70a */
        hkVector4_setTransformedInversePos(&rayFromLocal, &sii->m_transform, &segmentStart);
        hkVector4_setTransformedInversePos(&rayToLocal, &sii->m_transform, &segmentEnd);
        rayOutput.m_hitFraction = 1.0f;
        rayShapeKey0 = -1;
        rayShapeKey1 = -1;
        rayShapeKeyIndex = 0;
        hkpShape_castRay((hkpShape *)sii->m_shape->m_physicsShape.m_pntr, &rayHit,
                         (const hkpShapeRayCastInput *)&rayFromLocal, (hkpShapeRayCastOutput *)&rayOutput);
        rayHitFraction = rayOutput.m_hitFraction;
        if (rayHitFraction >= 1.0f)
            return;
    }

    /* --- classify the face by its slope relative to up --------------------- */
    faceSlopeDot = hkVector4_dot3fpu(&planeForwardAxis, &action->m_upDir);

    if (faceSlopeDot <= (float)(action->m_top.m_maxSlope * HK_DEG_TO_RAD))
    {
        if (faceSlopeDot >= (float)(action->m_bottom.m_maxSlope * -HK_DEG_TO_RAD))
        {
            /* near-vertical face: left vs right by tangent sign */
            if (faceSlopeDot <= 0.0f)
            {
                decorationList = &info->m_leftDecorations;
                faceInfo = &action->m_right;
            }
            else
            {
                decorationList = &info->m_rightDecorations;
                faceInfo = &action->m_left;
            }
        }
        else
        {
            /* steeply down-facing: top face */
            faceInfo = (FractureFaceInfo *)&action->m_top;
            decorationList = &info->m_topDecorations;
        }
    }
    else
    {
        /* steeply up-facing: bottom face */
        decorationList = &info->m_bottomDecorations;
        faceInfo = (FractureFaceInfo *)&action->m_bottom;
    }

    if (!decorationList->m_size)
        return;

    /* flip the frame handedness for the -X half of the face */
    {
        float handedness = hkVector4_dot3fpu(&planeUpAxis, &planeForwardAxis);
        if (handedness < 0.0f)
        {
            hkVector4 swap = planeRightAxis;
            planeRightAxis = planeForwardAxis;
            planeForwardAxis = swap;
            hkVector4_setNeg4(&planeUpAxis, &planeUpAxis);
        }
    }

    randomSeed = (int)hkVector4_dot3fpu(&segmentStart, &segmentStart);
    decorationTransform.m_translation = segmentStart;
    previousDecorationShape = NULL;

    /* --- scatter loop over the decoration list ----------------------------- */
    while (1)
    {
        unsigned int decorationCount = (unsigned int)decorationList->m_size;
        hkdShapeInstanceInfo *decorations = (hkdShapeInstanceInfo *)decorationList->m_data;
        unsigned char pivotMode = action->m_decorationPivot.m_storage;
        const hkdBreakableShape *decorationShape;
        const hkpShape *decorationPhysicsShape;
        float faceGap;

        randomState = LCG_MUL * (unsigned int)randomSeed + LCG_ADD;

        /* pick a decoration and seed an identity transform */
        decorationShape = decorations[(randomState >> 13) % decorationCount].m_shape;
        {
            hkVector4 zero = {{0.0f, 0.0f, 0.0f, 0.0f}};
            decorationTransform.m_rotation.m_col0 = zero;
            decorationTransform.m_rotation.m_col1 = zero;
            decorationTransform.m_rotation.m_col2 = zero;
            decorationTransform.m_rotation.m_col0.m_quad.___u0.v[0] = 1.0f;
            decorationTransform.m_rotation.m_col1.m_quad.___u0.v[1] = 1.0f;
            decorationTransform.m_rotation.m_col2.m_quad.___u0.v[2] = 1.0f;
            decorationTransform.m_translation = zero;
        }

        /* AABB_CENTER: seat the shape on its own AABB.
           DEVIATION: prior text referenced a non-existent DECORATION_PIVOT_SUPPORT
           enumerator; DB DecorationPivot has only GEOMETRY_PIVOT(0)/AABB_CENTER(1). */
        if (pivotMode == AABB_CENTER)
        {
            hkVector4 aabbShift;
            hkAabb decorationAabb;
            hkpShape_getAabb((hkpShape *)decorationShape->m_physicsShape.m_pntr, &decorationTransform, 0.0f, &decorationAabb);
            hkVector4_setSub(&aabbShift, &decorationTransform.m_translation,
                             &decorationTransform.m_translation);
            decorationTransform.m_translation = aabbShift;
        }

        decorationPhysicsShape = decorationShape->m_physicsShape.m_pntr;

        if (previousDecorationShape)
        {
            /* gap test: linear-cast this decoration against the previously placed
               one through the collision dispatcher's agent function. */
            const hkpShape *previousPhysicsShape = previousDecorationShape->m_physicsShape.m_pntr;
            hkpCollisionDispatcher *dispatcher = info->m_collisionInput.base.m_dispatcher.m_storage;
            unsigned int agentIndex =
                dispatcher->m_agent2Types[decorationPhysicsShape->m_type.m_storage][previousPhysicsShape->m_type.m_storage];
            void (__fastcall *linearCast)(const hkpCdBody *, const hkpCdBody *,
                                          const hkpLinearCastCollisionInput *,
                                          hkpCdPointCollector *, hkpCdPointCollector *) =
                dispatcher->m_agent2Func[agentIndex].m_linearCastFunc;
            float castLengthSq;

            if (!linearCast)
                return;

            hkpCollidable_ctor(&fixedCollidable, previousPhysicsShape,
                               &previousCollidableTransform, 0);
            hkpCollidable_ctor(&castCollidable, decorationPhysicsShape,
                               &decorationTransform, 0);

            hkVector4_setSub(&castDirection, &planeRightAxis, &planeForwardAxis);
            info->m_collisionInput.m_path = castDirection;

            pointCollector.m_vftable = &hkpClosestCdPointCollector_vftable;
            pointCollector.m_earlyOutDistance = 3.40282e38f;
            pointCollector.m_closestDistance = 3.40282e38f;
            pointCollector.m_hasHit = 0;

            castLengthSq = hkVector4_dot3fpu(&castDirection, &castDirection);
            info->m_collisionInput.base.m_tolerance.m_storage = 0.1f;
            info->m_collisionInput.m_cachedPathLength = castLengthSq; /* SDK normalizes */

            linearCast((const hkpCdBody *)&castCollidable, (const hkpCdBody *)&fixedCollidable,
                       &info->m_collisionInput, (hkpCdPointCollector *)&pointCollector, NULL);
            if (!pointCollector.m_hasHit)
                return;

            /* place the gap based on face info + cast contact distance */
            faceGap = faceInfo->m_gap * pointCollector.m_closestDistance;
            pointCollector.m_vftable = &hkpCdPointCollector_vftable;
            hkVector4_addScaledDir(&decorationTransform.m_translation, &castDirection, faceGap);
        }
        else
        {
            /* first decoration on this face: seat it against the piece surface
               using the piece's maximum projection along the (negated) right axis. */
            float surfaceProjection;
            float pieceProjection;
            hkVector4_setNeg4(&negatedRight, &planeRightAxis);
            surfaceProjection = hkVector4_dot3fpu(&decorationTransform.m_translation, &negatedRight);
            pieceProjection = hkpShape_getMaximumProjection(decorationPhysicsShape, &negatedRight);
            faceGap = pieceProjection - surfaceProjection;
            hkVector4_addScaledDir(&decorationTransform.m_translation, &negatedRight, faceGap);
        }

        /* --- reject placements that overhang the face along the forward axis - */
        {
            float decorationProjection;
            float clearance;
            hkVector4 surfaceDelta;
            (void)hkVector4_dot3fpu(&forwardProbe, &decorationTransform.m_translation);
            decorationProjection = hkpShape_getMaximumProjection(decorationPhysicsShape, &forwardProbe);
            hkVector4_setSub(&surfaceDelta, &planeUpAxis, &decorationTransform.m_translation);
            clearance = hkVector4_dot3fpu(&surfaceDelta, &forwardProbe);
            if ((float)(decorationProjection - clearance) > faceGap)
                return;
        }

        /* --- snap to the vertical grid ----------------------------------- */
        if (action->m_vertGridDist != 0.0f)
        {
            float alongUp = hkVector4_dot3fpu(&decorationTransform.m_translation, &planeUpAxis);
            float offsetFromGrid = alongUp - action->m_vertGridDist;
            double gridSteps = floor((double)((offsetFromGrid / action->m_vertGridDist) + 0.5f));
            float snap = (float)((float)gridSteps * action->m_vertGridDist) - offsetFromGrid;
            hkVector4_addScaledDir(&decorationTransform.m_translation, &planeUpAxis, snap);
        }

        /* --- snap to the horizontal grid --------------------------------- */
        if (action->m_horGridDist != 0.0f)
        {
            float alongRight = hkVector4_dot3fpu(&decorationTransform.m_translation, &planeRightAxis);
            float offsetFromGrid = alongRight - action->m_horGridDist;
            double gridSteps = floor((double)((offsetFromGrid / action->m_horGridDist) + 0.5f));
            float snap = (float)((float)gridSteps * action->m_horGridDist) - offsetFromGrid;
            hkVector4_addScaledDir(&decorationTransform.m_translation, &planeRightAxis, snap);
        }

        /* --- density test, then commit the placement --------------------- */
        randomSeed = (int)(LCG_MUL * randomState + LCG_ADD);
        if ((float)((float)(unsigned int)randomSeed * LCG_TO_UNIT_FLOAT) < faceInfo->m_density)
        {
            const hkdGraphicsShape *decorationGraphics = decorationShape->m_graphicsShape.m_pntr;
            int graphicsIndex;
            BodyInfo_Graphics *graphicsEntry;

            if (biA->m_graphics.m_size == (biA->m_graphics.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&biA->m_graphics, 80);

            graphicsIndex = biA->m_graphics.m_size;
            biA->m_graphics.m_size = graphicsIndex + 1;
            graphicsEntry = &biA->m_graphics.m_data[graphicsIndex];
            graphicsEntry->m_transform = decorationTransform;
            graphicsEntry->m_graphics = decorationGraphics;
        }

        /* remember this placement for the next gap test */
        previousCollidableTransform = decorationTransform;
        previousDecorationShape = decorationShape;
        randomSeed = (int)randomState;
    }
}
