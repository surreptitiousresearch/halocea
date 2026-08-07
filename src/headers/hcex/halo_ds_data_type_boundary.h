#pragma once
// Boundary declarations for the ds_data type-policy helpers that the halo* ds_data type
// instantiations (dsDATA_TYPE_IMPL / dsDATA_TYPE_STATIC over haloRAW_INPUT_X360_ENV and
// haloPLAYER_ACTIONS_ENV) forward to. None of these are reversed in this batch; they are the
// next frontier. Signatures are DB-verified against the mangled names.
#include "../ws/ds/dsDATA_TYPE.h"
#include "../ws/ds/dsCONVERTION_TYPE.h"
#include "../ws/ds/dsDATA_CMP_RES.h"
#include "../ws/ds/dsDATA_MATH.h"

// The two per-T policy templates this header used to re-declare have a single canonical body each.
// Redeclaring them here made both a file-scope redefinition (`error: redefinition` in the
// header_layout probe TU) and let the visible signature depend on include order:
//   dsDATA_TYPE_CONSTRUCT<T> — the canonical carries the union of the three former bodies; the
//       Construct/CopyConstruct/GetPtr-overload declarations this header contributed are in it.
//   dsDATA_TYPE_CLASS<T>     — the canonical is the superset (Get/SetProperty, declared here,
//       plus IsFunc/IsProperty/CallFunc).
#include "../ws/ds/dsDATA_TYPE_CONSTRUCT.h"
#include "../ws/ds/dsDATA_TYPE_CLASS.h"   /* also supplies the complete dsSTRID */

struct dsDATA;   // ws/ds/dsDATA.h

namespace ds_data {

// Global dispatch-table lookups keyed by (dstTypeId, srcTypeId[, convType]); return null when no
// converter/comparator is registered for the requested direction.
int (*GetConverter(int dstTypeId, int srcTypeId, dsCONVERTION_TYPE convType))
        (const unsigned int *src, unsigned int *dest);
int (*GetRawConverter(int dstTypeId, int srcTypeId, dsCONVERTION_TYPE convType))
        (const void *src, void *dest);
dsDATA_CMP_RES (*GetComparator(int dstTypeId, int srcTypeId))
        (const unsigned int *left, const unsigned int *right);
const dsDATA_TYPE *GetMTSafeTypeAnalog(int typeId);

// Math-operator dispatch-table lookups keyed by (op, dstTypeId[, srcTypeId]); return null when no
// operator is registered for the requested direction.
dsDATA_MATH_RES (*GetBinaryOp(int op, int dstTypeId, int srcTypeId))
        (const unsigned int *left, const unsigned int *right, dsDATA &ret);
dsDATA_MATH_RES (*GetUnaryOp(int op, int typeId))
        (const unsigned int *data, dsDATA &ret);

} // namespace ds_data
