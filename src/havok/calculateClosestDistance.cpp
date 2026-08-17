/* calculateClosestDistance @0x83A49928 */
#include "../headers/havok/hkpCollisionDispatcher.h"
#include "../headers/havok/hkContactPoint.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkpShape.h" /* hkpShape used by-pointer in CdBodyScratch */

/* Boundary types/data for the query scaffolding (2-level descent limit). */
#include "../headers/havok/hkpNullCollisionFilter.h" /* full 48-byte layout: filter is a by-value local */
extern void *hkpClosestCdPointCollector_vftable[]; /* hkpClosestCdPointCollector::`vftable' */
extern void *hkpCdPointCollector_vftable[];        /* hkpCdPointCollector::`vftable' */

/* Reconstructed stack scaffolding matching the DB body's frame layout. */
/* hkpCdBody (16 bytes as used here): shape, shape key (-1), transform, parent. */
typedef struct CdBodyScratch
{
    const hkpShape *m_shape;      /* 0  */
    int m_shapeKey;               /* 4  (-1 = root) */
    const hkTransform *m_motion;  /* 8  */
    const void *m_parent;         /* 12 (0 = none) */
} CdBodyScratch;

/* hkpClosestCdPointCollector (as used): vtable, running closest distance, the
   captured contact point, and a hit flag. Offsets follow the DB frame. */
typedef struct ClosestPointCollector
{
    void *__vftable;         /* 0x00 */
    float m_earlyOutDistance;/* 0x04 */
    float m_maxDistance;     /* 0x08 */
    unsigned char _pad0C[36];/* 0x0C */
    hkContactPoint m_point;  /* 0x30 */
    int m_hasHit;            /* 0x50 */
} ClosestPointCollector;

/* The AgentFuncs closest-point call receives a collision-input header carrying
   the dispatcher, tolerance and filter. */
typedef struct ClosestPointInput
{
    hkpCollisionDispatcher *m_dispatcher; /* 0x00 */
    float m_tolerance;                    /* 0x04 */
    void *m_filter;                       /* 0x08 */
    unsigned char _pad0C[68];             /* pad to the DB frame size */
} ClosestPointInput;

/* calculateClosestDistance — query the collision dispatcher for the closest point
   between shapes a and b at their given transforms, within maxDistance. Looks up
   the pair's agent via m_agent2Types[typeA][typeB], invokes the agent's
   getClosestPointFunc with a closest-point collector, and — on a hit — copies the
   resulting contact point into contactPointOut. Returns 1 when a point was found,
   0 otherwise (including when no agent is registered for the pair).

   DEVIATION: the collector/CdBody/input frames are reconstructed from the DB
   stack layout; the final contact-point copy is a VMX128 two-vector store,
   reproduced here as a structure copy. */
/* DEVIATION: 7 args — the prologue reads exactly r3-r7, r9 and f1
   (0x83A49938-54); r8 is the float-slot shadow burned by maxDistance(f1) and
   r10 is never read. A previously-declared trailing `int unused` 8th parameter
   was a transcribed Hex-Rays phantom and has been removed. */
extern "C" int calculateClosestDistance(hkpCollisionDispatcher *dispatcher, const hkpShape *a,
                             const hkTransform *transA, const hkpShape *b,
                             const hkTransform *transB, float maxDistance,
                             hkContactPoint *contactPointOut)
{
    hkpNullCollisionFilter filter;
    unsigned int typeA, typeB;
    int agentIndex;
    void (*getClosestPointFunc)(const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpCdPointCollector *);
    CdBodyScratch bodyA, bodyB;
    ClosestPointCollector collector;
    ClosestPointInput input;

    hkpNullCollisionFilter_construct(&filter);
    typeA = a->m_type.m_storage;
    typeB = b->m_type.m_storage;

    input.m_dispatcher = dispatcher;
    input.m_tolerance = maxDistance;
    input.m_filter = &filter;

    agentIndex = dispatcher->m_agent2Types[typeA][typeB];
    getClosestPointFunc = dispatcher->m_agent2Func[agentIndex].m_getClosestPointFunc; // types match; no cast

    if (!getClosestPointFunc)
    {
        hkpNullCollisionFilter_destruct(&filter);
        return 0;
    }

    bodyA.m_shape = a;
    bodyA.m_shapeKey = -1;
    bodyA.m_motion = transA;
    bodyA.m_parent = 0;
    bodyB.m_shape = b;
    bodyB.m_shapeKey = -1;
    bodyB.m_motion = transB;
    bodyB.m_parent = 0;

    collector.__vftable = (void *)hkpClosestCdPointCollector_vftable;
    collector.m_earlyOutDistance = 3.40282e38f;
    collector.m_maxDistance = 3.40282e38f;
    collector.m_hasHit = 0;

    // Scratch frames are DB-reconstructed stand-ins for the real hkp* types; object-cast them
    // to the agent function's declared parameter types (no function-pointer cast).
    getClosestPointFunc((const hkpCdBody *)&bodyA, (const hkpCdBody *)&bodyB,
                        (const hkpCollisionInput *)&input, (hkpCdPointCollector *)&collector);

    /* The collector's virtual dispatch is finished; drop it back to the base
       vtable (matching the compiler-generated collector teardown). */
    collector.__vftable = (void *)hkpCdPointCollector_vftable;

    if (collector.m_hasHit)
    {
        *contactPointOut = collector.m_point;
        hkpNullCollisionFilter_destruct(&filter);
        return 1;
    }

    hkpNullCollisionFilter_destruct(&filter);
    return 0;
}
