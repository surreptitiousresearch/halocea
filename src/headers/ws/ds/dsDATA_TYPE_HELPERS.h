#pragma once
#include "dsBIT_STREAM.h"
struct dsDATA;   // ws/ds/dsDATA.h — pass-through user context (pointer/ref only)
struct dsSTRID;  // ws/ds/dsSTRID.h — interned id (by value)
struct fioFILE;  // ws/fio/fioFILE.h — boundary (pointer only)

// Static-policy helper templates in the ds_data type machinery. Each concrete dsDATA_TYPE_IMPL<T>
// virtual override forwards to the matching static here. Names are the DB-authoritative mangled
// class names (?$dsDATA_TYPE_STREAM / _CONSTRUCT / _PACK, and dsDATA_TYPE_CLASS as the reflection
// dispatch target). Bodies are reversed per concrete T as explicit specializations under src/hcex/.
namespace ds_data {

// dsDATA_TYPE_STREAM<T> — disk (fioFILE) serialization policy. For plain value types both
// Save/Load are the "unsupported" stubs (STRONG_ASSERT + return 0), per ds_data_type_impl.hpp.
template<class T>
struct dsDATA_TYPE_STREAM {
    static int Save(const T &data, fioFILE *pFile);
    static int Load(fioFILE *pFile, T &data);
};

// dsDATA_TYPE_CONSTRUCT<T> — heap value-cell lifetime policy. The storage cell holds a pointer to
// the boxed value; the mangled param type is `unsigned long*` (the 4-byte cell), which the bodies
// dereference as void** — declared as void** here to match those bodies.
template<class T>
struct dsDATA_TYPE_CONSTRUCT {
    static void Destroy(void **storage);              // ?Destroy@...@@SAXPAK@Z
    static void Copy(const void **src, void **dest);  // ?Copy@...@@SAXPBKPAK@Z
    static void CopyObj(const T *obj, void **dest);   // ?CopyObj@...@@SAXPBU<T>@@PAK@Z
};

// dsDATA_TYPE_PACK<T> — network (ds::BIT_STREAM) serialization policy; forwards to T's own
// Pack/Unpack members.
template<class T>
struct dsDATA_TYPE_PACK {
    static void Pack(const T &data, ds::BIT_STREAM &stream, const dsDATA &userData);
    static void Unpack(ds::BIT_STREAM &stream, T &data, const dsDATA &userData);
};

// dsDATA_TYPE_CLASS<T> — reflection / SSL-scripting dispatch policy; the IMPL IsFunc/IsProperty/
// CallFunc virtuals forward here. Signatures taken from the mangled IMPL virtuals (const dsDATA&,
// dsSTRID by value). boundary — bodies not reversed in this batch.
template<class T>
struct dsDATA_TYPE_CLASS {
    static int IsFunc(const dsDATA &inst, dsSTRID id);
    static int IsProperty(const dsDATA &inst, dsSTRID id);
    static int CallFunc(dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval);
};

} // namespace ds_data
