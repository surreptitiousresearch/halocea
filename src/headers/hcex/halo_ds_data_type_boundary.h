#pragma once
// Boundary declarations for the ds_data type-policy helpers that the halo* ds_data type
// instantiations (dsDATA_TYPE_IMPL / dsDATA_TYPE_STATIC over haloRAW_INPUT_X360_ENV and
// haloPLAYER_ACTIONS_ENV) forward to. None of these are reversed in this batch; they are the
// next frontier. Signatures are DB-verified against the mangled names.
#include "../ws/ds/dsDATA_TYPE.h"
#include "../ws/ds/dsCONVERTION_TYPE.h"
#include "../ws/ds/dsDATA_CMP_RES.h"
#include "../ws/ds/dsDATA_MATH.h"

struct dsSTRID;  // ws/ds/dsSTRID.h
struct dsDATA;   // ws/ds/dsDATA.h

namespace ds_data {

// Construction/storage policy for a stored type T (inline slot vs. boxed). The STATIC forwarders
// delegate value lifetime + pointer access to these.
template<class T>
struct dsDATA_TYPE_CONSTRUCT {
    static void Construct(unsigned int *storage, const T *obj);        // construct slot from an object
    static void CopyConstruct(unsigned int *storage, const unsigned int *data); // construct slot from a slot
    static void Destroy(unsigned int *storage);
    static void Copy(const unsigned int *src, unsigned int *dest);
    static void CopyObj(const T *obj, unsigned int *dest);
    static const T *GetPtr(const unsigned int *storage);
    static T       *GetPtr(unsigned int *storage);
};

// Named-member/property policy for a stored type T. The IMPL Get/SetProperty overrides delegate here.
template<class T>
struct dsDATA_TYPE_CLASS {
    static int GetProperty(dsSTRID id, const dsDATA &inst, dsDATA &val);
    static int SetProperty(dsSTRID id, dsDATA &inst, const dsDATA &val);
};

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
