#pragma once
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "dsTSTRING.h"
#include "dsDATA_CMP_RES.h"
#include "dsDATA_MATH.h"

struct dsSTRID;  // ws/ds/dsSTRID.h
struct dsDATA;   // ws/ds/dsDATA.h
struct fioFILE;  // ws/fio/fioFILE.h — boundary (pointer only)
namespace ds { struct BIT_STREAM; } // ws/ds/dsBIT_STREAM.h — boundary (ref only)

namespace ds_data {

// Monotonic type-id counter and the type-registry sink, both stamped by every
// dsDATA_TYPE_IMPL<T> constructor (DB 0x823BCAF8). Boundary — defined in the ds_data runtime.
extern int gTypeId;                          // ?gTypeId@ds_data@@3HA
void RegisterType(const dsDATA_TYPE *type);  // ?RegisterType@ds_data@@YAXPBVdsDATA_TYPE@@@Z

// Concrete implementation of the dsDATA_TYPE interface for a stored C++ type T.
// DB-verified layout (types dsDATA_TYPE_IMPL<T>): __vftable@0 (inherited) + id@4 — size 8.
// Method bodies are reconstructed per concrete T in the corresponding .cpp as
// explicit specializations.
template<class T>
struct dsDATA_TYPE_IMPL : dsDATA_TYPE {
    int id; // 0x04 — runtime type id assigned from ds_data::gTypeId++ at construction

    dsDATA_TYPE_IMPL(); // stamps id, installs the IMPL vtable, and self-registers (RegisterType)

    void  Construct(unsigned int *storage) const;  // overrides dsDATA_TYPE (virtual)
    void  CopyConstruct(unsigned int *storage, const unsigned int *data) const;  // overrides dsDATA_TYPE (virtual)
    void  ConstructFromObj(unsigned int *storage, const void *obj) const;  // overrides dsDATA_TYPE (virtual)
    void  Destroy(unsigned int *storage) const;  // overrides dsDATA_TYPE (virtual)
    void *GetPtr(unsigned int *storage) const;  // overrides dsDATA_TYPE (virtual)
    const void *GetPtr(const unsigned int *storage) const;  // const overload (virtual)
    // Assign-from-object (vtable slot 0x14); forwards to dsDATA_TYPE_CONSTRUCT<T>::CopyObj.
    void  CopyObj(const void *obj, unsigned int *dest) const;  // overrides dsDATA_TYPE (virtual)
    // Copy an already-constructed value between storage slots (dsDATA_TYPE_CONSTRUCT<T>::Copy).
    void  Copy(const unsigned int *src, unsigned int *dest) const;  // overrides dsDATA_TYPE (virtual)
    int   Convert(const unsigned int *src, unsigned int *dest,
                  const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const;  // overrides dsDATA_TYPE (virtual)
    // Raw (unboxed) conversion path; looks up ds_data::GetRawConverter and runs it.
    int   RawConvert(const void *src, void *dest,
                     const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const;  // (virtual)
    // The type's MT-safe replacement descriptor (ds_data::GetMTSafeTypeAnalog).
    const dsDATA_TYPE *MTSafeAnalog() const;  // (virtual)
    // Three-way compare against a value of `type`; ds_data::GetComparator dispatch.
    dsDATA_CMP_RES Compare(const unsigned int *left, const unsigned int *right,
                           const dsDATA_TYPE *type) const;  // (virtual)
    // Value (de)serialization to fioFILE. This T has no serializer: strong-assert + fail.
    int   Save(const unsigned int *storage, fioFILE *pFile) const;  // (virtual)
    int   Load(fioFILE *pFile, unsigned int *storage) const;        // (virtual)
    // Network (un)pack; forwards to T::Pack / T::Unpack over the stored value.
    void  NetPack(const unsigned int *storage, ds::BIT_STREAM &stream, const dsDATA &userData) const;  // (virtual)
    void  NetUnpack(ds::BIT_STREAM &stream, unsigned int *storage, const dsDATA &userData) const;       // (virtual)
    // Binary/unary math dispatch (ds_data::GetBinaryOp / GetUnaryOp lookups).
    dsDATA_MATH_RES DoMathOp(dsDATA_BINARY_OP op, const unsigned int *left, const unsigned int *right,
                             const dsDATA_TYPE *type, dsDATA &ret) const;  // (virtual)
    dsDATA_MATH_RES DoMathOp(dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret) const;  // (virtual)

    // Named-property accessors (virtual overrides); forward to dsDATA_TYPE_CLASS<T>.
    int   GetProperty(dsSTRID id, const dsDATA &inst, dsDATA &val) const;
    int   SetProperty(dsSTRID id, dsDATA &inst, const dsDATA &val) const;
    // IMPL-specific MT-safety probe over a stored value (distinct from dsDATA_TYPE::IsMTSafe()).
    int   IsMTSafe(const unsigned int *storage) const;
    // Reflection / scripting virtuals — forward to the matching static in dsDATA_TYPE_CLASS<T>.
    int   IsFunc(const dsDATA &inst, dsSTRID id) const;
    int   IsProperty(const dsDATA &inst, dsSTRID id) const;
    int   CallFunc(dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval) const;

    // Virtual: format a boxed value for debug display. For types with no bespoke formatter this
    // forwards to dsDATA_TYPE_DBG<T> and yields the literal "undef". Returns by value (sret).
    dsTSTRING<char> GetDbgString(const unsigned int *storage) const;
};

} // namespace ds_data
