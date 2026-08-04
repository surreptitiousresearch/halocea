#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
#include "dsDATA_TYPE.h"
#include "dsCONVERTION_TYPE.h"
#include "dsDATA_CMP_RES.h"
#include "dsDATA_MATH.h"
struct fioFILE; // ws/fio/fioFILE.h — boundary here (only pointer/ref passed through)
struct dsDATA;  // ws/ds/dsDATA.h — boundary here (only ref passed through)
namespace ds { struct BIT_STREAM; } // ws/ds/dsBIT_STREAM.h — boundary here
// Boundary declarations for ds_data helpers called but not reversed in this batch.

// One cell of the global type-to-type conversion table t2tTable[dstTypeId][srcTypeId].
// DB-derived: converter@0, convertFlags@8; cell stride 0x70 (112) bytes (disasm at 0x8256DC64).
// Inner (src-type) dimension = 32 (0x5400 = int-row base = 6 rows * 32 cells * 112 bytes).
typedef struct dsT2T_ENTRY {
    int (*converter)(const unsigned int *src, unsigned int *dest); // 0x00 conversion fn (null = none)
    unsigned char pad_04[4];                                       // 0x04
    unsigned int  convertFlags;                                    // 0x08 bitmask over dsCONVERTION_TYPE
    unsigned char pad_0C[104];                                     // 0x0C remainder of the 112-byte cell
} dsT2T_ENTRY;

// t2tTable @ 0x841B4A90 — global conversion dispatch matrix indexed [dstTypeId][srcTypeId].
extern dsT2T_ENTRY t2tTable[][32];

// dsDATA_TYPE_STORAGE<T>::dataType — the singleton runtime type descriptor for the stored type T
// (?dataType@?$dsDATA_TYPE_STORAGE@...@@2PBVdsDATA_TYPE@@B, global namespace). Boundary — the
// descriptor object itself is not reversed; only its address is compared/passed here.
template<class T>
struct dsDATA_TYPE_STORAGE {
    static const dsDATA_TYPE *const dataType;
};

namespace ds_data {

// dsDATA_TYPE_STATIC<T>: static-policy twin of dsDATA_TYPE_IMPL<T>; the IMPL virtual
// overrides forward to these static entry points.
template<class T>
struct dsDATA_TYPE_STATIC {
    static void Construct(unsigned int *storage);                        // boundary — not reversed
    static int  Convert(const unsigned int *src, unsigned int *dest,
                        const dsDATA_TYPE *type, dsCONVERTION_TYPE convType); // boundary — not reversed
    // Additional static entry points forwarded to by the IMPL virtual overrides. Reversed as
    // explicit specializations in src/hcex/<T>__*.cpp (thin forwarders to dsDATA_TYPE_CONSTRUCT<T>
    // / the ds_data dispatch helpers).
    static void CopyConstruct(unsigned int *storage, const unsigned int *data); // construct slot from a slot
    static void ConstructFromObj(unsigned int *storage, const T *obj);           // construct slot from an object
    static void Destroy(unsigned int *storage);
    static void Copy(const unsigned int *src, unsigned int *dest);
    static void CopyObj(const T *obj, unsigned int *dest);
    static const T *GetPtr(const unsigned int *storage);
    static T       *GetPtr(unsigned int *storage);
    static int  RawConvert(const void *src, void *dest,
                           const dsDATA_TYPE *type, dsCONVERTION_TYPE convType);
    static const dsDATA_TYPE *MTSafeAnalog();
    static dsDATA_CMP_RES Compare(const unsigned int *left, const unsigned int *right,
                                  const dsDATA_TYPE *type);
    static int Save(const T &data, fioFILE *pFile);
    static int Load(fioFILE *pFile, T &data);
    // Network (de)serialization: thin forwarders to the stored type's own Pack/Unpack.
    static void NetPack(const T &data, ds::BIT_STREAM &stream, const dsDATA &userData);
    static void NetUnpack(ds::BIT_STREAM &stream, T &data, const dsDATA &userData);
    // Type-erased math dispatch: look up a registered operator (dstTypeId 0) and run it.
    static dsDATA_MATH_RES DoMathOp(dsDATA_BINARY_OP op, const unsigned int *left,
                                    const unsigned int *right, const dsDATA_TYPE *type, dsDATA &ret);
    static dsDATA_MATH_RES DoMathOp(dsDATA_UNARY_OP op, const unsigned int *data, dsDATA &ret);
};

// dsDATA_DEFAULT_VALUE<T>: supplies the default-constructed value for a stored type.
// GetDefaultVal() returns a reference to a function-static default instance.
template<class T>
struct dsDATA_DEFAULT_VALUE {
    static const T &GetDefaultVal();                                     // boundary — not reversed
};

} // namespace ds_data

namespace ds {

// ds::FindPrime(int) @ boundary — round `minPrime` up to the next prime in a fixed internal
// table (used to size hash-table bucket arrays, e.g. dsSTRID_POOL::dsSTRID_POOL's 4096-bucket
// presize); body not reversed here.
int FindPrime(int minPrime); // boundary

// ds::ByteReorder @ ?ByteReorder@ds@@YAXPAEHH@Z — in-place reverse bytes within `reorderGain`-sized
// groups (endian fixup for buffered reads). boundary
void ByteReorder(unsigned char *data, int size, int reorderGain); // boundary

} // namespace ds
