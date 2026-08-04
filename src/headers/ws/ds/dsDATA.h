#pragma once
#include "dsDATA_TYPE.h"
#include "dsDATA_MATH.h"
#include "dsTSTRING.h"
// ws-engine ds: a type-erased value — a machine-word of storage plus the runtime type
// descriptor that knows how to construct/destroy/convert it.
// DB-verified layout (types_members dsDATA): storage@0, type@4 — size 8.

typedef struct dsDATA {
    unsigned int         storage; // 0x00 raw value (or pointer to heap value)
    const dsDATA_TYPE   *type;    // 0x04 runtime type descriptor (null when empty)

    // Trivial default: kept explicit (rather than letting the new typed constructor below
    // suppress it) because many existing translation units in this corpus declare a bare
    // `dsDATA x;` and self-initialise `x.type = nullptr;` afterward -- preserving that
    // uninitialised-until-assigned behavior avoids silently changing their semantics.
    dsDATA() = default;

    // 0x82516B70 — construct an empty value of `_type` in place (adopt the descriptor, then
    // dispatch its Construct virtual to default-construct into `storage`).
    dsDATA(const dsDATA_TYPE *_type);

    // 0x823B3F78 — destroy the stored value (dispatches the stored type's Destroy virtual) when
    // non-empty.
    // CAVEAT: several dsDATA-consuming .cpp files reversed earlier in this corpus (e.g.
    // sslOBJ_REF__Set.cpp, sslSCRIPTED_FUNCTION_BASE__ExecCode.cpp) manually write
    // `if (x.type) x.type->Destroy(&x.storage);` before each return for a local dsDATA -- that
    // is this destructor's body, inlined at each of the local's exit points, predating this
    // ctor/dtor reversal pass. With ~dsDATA() now modeled explicitly, a literal recompile would
    // invoke Destroy twice for those locals (once via the manual call, once automatically at
    // scope exit). Not fixed here (single-function target for this batch) -- a follow-up pass
    // should drop the now-redundant manual calls in those sibling files.
    ~dsDATA();

    // 0x823CFE20 (SetValue<int>) — set this value to `*val`: if empty, adopt the T type descriptor
    // and construct from `val`; if it already holds T, overwrite in place; otherwise construct a
    // fresh T from `val`, convert it into this's existing type, and (on success) replace this
    // value. Reversed per-instantiation.
    template<class T>
    void SetValue(const T *val);

    // Install `val` as this dsDATA's current value (per-type descriptor `dsDATA_TYPE_STORAGE<T>`).
    // Reversed per-instantiation in src/ws/ds/dsDATA__StoreValue_*.cpp.
    template<class T>
    void StoreValue(const T &val);

    // 0x823B3FA0 — overwrite this value+type by copying from another dsDATA `src`: if src is
    // empty, drop this value; if this is empty, copy-construct from src; if same type, copy-assign
    // in place; otherwise destroy the old value, adopt src's type, and copy-construct.
    void StoreValue(const dsDATA &src);

    // 0x82A41CA8 — overwrite this value+type by copying from another dsDATA (drops the old
    // value, installs src's type descriptor and a copy of its value). Body not decompiled here.
    void SetValue(const dsDATA *src); // boundary

    // 0x823CD570 — overwrite this value+type by copying from `src` (by reference): if src is
    // empty, drop this value; if this is empty, copy-construct from src; if same type, in-place
    // Copy; otherwise construct a fresh value, Convert src into it, then destroy the old value and
    // adopt the result. Semantically the reference-argument twin of StoreValue(const dsDATA&).
    void SetValue(const dsDATA &src);

    // 0x823B34B8 (GetValue<bool>) — retrieve the stored value as a T. Fast path when the stored
    // type already is T (a direct GetPtr read); otherwise dispatch the runtime converter and
    // return whether a converter existed. `convType` selects the conversion direction/kind.
    template<class T>
    int GetValue(T &val, dsCONVERTION_TYPE convType) const;

    // True when this dsDATA holds no value (null type descriptor). boundary
    bool IsNull() const;

    // 0x82A41B40 — apply unary operator `op` to this value in place (dispatched through the
    // stored type's DoMathOp). Returns whether an operator existed and produced a result.
    dsDATA_MATH_RES Math(dsDATA_UNARY_OP op);

    // 0x82AD7B98 — apply BINARY operator `op` combining this value with `right`, writing the
    // result into `ret`. Dispatches this type's binary DoMathOp; on DSD_MATH_UNDEF it retries by
    // constructing a scratch of one operand's type, converting the other operand into it
    // (DSD_CONV_MATH), and dispatching again (this-type first, then right-type). const (QBA).
    dsDATA_MATH_RES Math(dsDATA_BINARY_OP op, const dsDATA &right, dsDATA &ret) const;

    // 0x823C2B30 — convert this value to `type` in place: constructs a fresh value of `type`,
    // dispatches the stored type's converter, and on success replaces this value+type. Returns 1
    // on success (or when already that type), 0 on failure / when either type is null.
    int Convert(const dsDATA_TYPE *type, dsCONVERTION_TYPE convType);

    // 0x82612C70 — make the stored value thread-safe: returns 1 when empty or already MT-safe;
    // otherwise converts this value in place to the stored type's declared MT-safe analog.
    int MakeMTSafe();

    // 0x826219F8 — produce a human-readable debug string for the stored value (const, returns a
    // dsTSTRING<char> by value / sret). Empty values render as "null".
    dsTSTRING<char> GetDbgString() const;

    // boundary (usage-inferred; 2026-07-31) — raw pointer to the wrapped value storage; the ssl
    // VM reads the underlying value (e.g. *(dsTSTRING<char>*)) out of a dsDATA via this. Not in
    // the DB under this name; usage from sslSCRIPTED_FUNCTION_BASE::ExecCode.
    void *RawWrapperPtr() const;
} dsDATA;
