#pragma once
#include "hkpShape.h"
#include "hkTransform.h"
#include "hkAabb.h"

typedef struct hkClass hkClass;
typedef struct hkStatisticsCollector hkStatisticsCollector;
typedef struct hkVector4 hkVector4;
typedef struct hkBool hkBool;
typedef struct hkpShapeRayCastInput hkpShapeRayCastInput;
typedef struct hkpShapeRayCastOutput hkpShapeRayCastOutput;

/* hkpShape virtual table (DB types_members hkpShape_vtbl). Only the slots the
   reversed non-virtual wrappers dispatch through are typed precisely; the rest
   are named for completeness of the layout. DEVIATION on argument order for
   getAabbImpl/castRayImpl reflects the real PPC call-site ABI (struct return of
   hkBool in r3 for castRay; tolerance passed after the hkAabb* out for getAabb),
   which differs from IDA's slot signature guess. */
typedef struct hkpShape_vtbl
{
    void (*dtor)(hkpShape *self);                                              /* 0x00 */
    const hkClass *(*getClassType)(hkpShape *self);                            /* 0x04 */
    void (*calcContentStatistics)(hkpShape *self, hkStatisticsCollector *, const hkClass *); /* 0x08 */
    float (*getMaximumProjection)(hkpShape *self, const hkVector4 *direction); /* 0x0C */
    const void *(*getContainer)(hkpShape *self);                              /* 0x10 */
    int (*isConvex)(hkpShape *self);                                          /* 0x14 */
    int (*calcSizeForSpu)(hkpShape *self, const void *, int);                 /* 0x18 */
    void (*getAabbImpl)(hkpShape *self, const hkTransform *localToWorld,
                        hkAabb *out, float tolerance);                        /* 0x1C */
    hkBool *(*castRayImpl)(hkpShape *self, hkBool *result,
                           const hkpShapeRayCastInput *input,
                           hkpShapeRayCastOutput *output);                    /* 0x20 */
} hkpShape_vtbl;

#define HKP_SHAPE_VTBL(shape) ((hkpShape_vtbl *)((shape)->base.__vftable))
