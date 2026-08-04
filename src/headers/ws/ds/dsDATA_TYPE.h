#pragma once
#include "dsCONVERTION_TYPE.h"
#include "dsDATA_MATH.h"
#include "dsSTRID.h" // dsSTRID passed by value to IsProperty/CallFunc (added 2026-07-31)
struct dsDATA; // forward (dsDATA includes this header)
template<class T> struct dsTSTRING; // forward — used only in the GetDbgString signature
// ws-engine ds_data: abstract runtime data-type interface.
// DB-verified layout (types_members dsDATA_TYPE): __vftable@0 — size 4.
// Concrete behavior is supplied by ds_data::dsDATA_TYPE_IMPL<T>. The virtuals below are
// recovered from the reversed IMPL overrides and are matched by NAME (the IMPL files use
// `override`); they are NOT in binary slot order. The DB-authoritative vtable order lives in
// `dsDATA_TYPE_vtbl` (29 slots): Construct@0, CopyConstruct@4, ConstructFromObj@8,
// Destroy@0xC, Copy@0x10, CopyObj@0x14, GetPtr@0x18, ... Convert@0x38. Container teardown
// (dsVECTOR<dsDATA>/dsVECTOR<dsPARAM>::Clear) dispatches Destroy at slot 0xC.

struct dsDATA_TYPE_vtbl; // boundary — ds subsystem vtable (29 slots, see comment above)

typedef struct dsDATA_TYPE {
    dsDATA_TYPE_vtbl *__vftable; // 0x00

    void  Construct(unsigned int *storage) const; // (virtual)

    // vtable slot 0x4 — construct a fresh value into `destStorage` by copying an existing
    // constructed value in `srcStorage` (copy-construct-from-value). Used by
    // dsDATA::StoreValue(const dsDATA&) on the empty and type-change paths.
    void  CopyConstruct(unsigned int *destStorage,
                                const unsigned int *srcStorage) const;

    void  ConstructFromObj(unsigned int *storage, const void *obj) const;
    void  Destroy(unsigned int *storage) const;

    // vtable slot 0x10 — overwrite an already-constructed value in `destStorage` from another
    // already-constructed value in `srcStorage` (copy-assign-from-value). Used by
    // dsDATA::StoreValue(const dsDATA&) on the same-type fast path.
    void  Copy(const unsigned int *srcStorage, unsigned int *destStorage) const;

    void *GetPtr(unsigned int *storage) const;

    // vtable slot 0x14 — overwrite an already-constructed value in `storage` from `obj`
    // (assign, not construct). Used by dsDATA::StoreValue on the same-type fast path.
    void  CopyObj(const void *obj, unsigned int *storage) const;
    int   Convert(const unsigned int *src, unsigned int *dest,
                          const dsDATA_TYPE *type, dsCONVERTION_TYPE convType) const;

    // vtable slot 0x2C (44) — read the named member `memberId` out of a struct value in `storage`
    // into `out`; returns nonzero on success. boundary — dispatched from sslSTACK_DATA::GetAsDsData.
    int   GetMember(const char *memberId, const unsigned int *storage, dsDATA *out) const; // boundary
    // vtable slot 0x30 (48) — store `val` into the named member `memberId` of a struct value in
    // `storage`; returns nonzero on success. boundary — dispatched from sslSTACK_DATA::SetRefTo.
    int   SetMember(const char *memberId, unsigned int *storage, const dsDATA *val) const; // boundary

    // vtable slot 0x5C — runtime type-id used to index the type-to-type conversion table
    // (0 for the null/untyped case). Reached via dsDATA_TYPE_STATIC<T>::Convert.
    int   GetTypeId() const;

    // Apply unary operator `op` to `operand`, writing the result into `result`; returns whether
    // an operator existed and succeeded. Dispatched from dsDATA::Math. Exact vtable slot not
    // verified in this batch (matched by name, like the other virtuals above). // boundary
    dsDATA_MATH_RES DoMathOp(dsDATA_UNARY_OP op, const dsDATA *operand,
                                     dsDATA *result) const;

    // Apply BINARY operator `op` combining the value in `leftStorage` (of THIS dispatching type)
    // with the value in `rightStorage` (of `rightType`), writing the result into `result`; returns
    // whether an operator existed and succeeded (DSD_MATH_UNDEF when this type has no binary op for
    // these operand types). Dispatched from dsDATA::Math(binary). Distinct vtable slot from the
    // unary DoMathOp (decompiler names it DoMathOp_2); matched by name. // boundary
    dsDATA_MATH_RES DoMathOp(dsDATA_BINARY_OP op, const unsigned int *leftStorage,
                                     const unsigned int *rightStorage,
                                     const dsDATA_TYPE *rightType, dsDATA *result) const;

    // Reached from dsDATA::MakeMTSafe — matched by name (slots not verified in this batch).
    int IsMTSafe() const;                    // boundary — nonzero if the type is MT-safe
    const dsDATA_TYPE *MTSafeAnalog() const; // boundary — the type's MT-safe replacement

    // vtable slot 0x68 (104) — produce a debug string for a value of this type; sret by value.
    dsTSTRING<char> GetDbgString() const;    // boundary

    // Human-readable type name (e.g. "int", "bool"); used in diagnostic messages. boundary — not
    // reversed in this batch (exact vtable slot unverified; matched by name).
    const char *GetDbgName() const; // boundary

    // boundary (usage-inferred; DB IMPL overrides use (const dsDATA&,dsSTRID)) (2026-07-31) —
    // dsDATA member-access dispatch as reached from sslSCRIPTED_FUNCTION_BASE::ExecCode through a
    // const dsDATA_TYPE*. Declared NON-virtual (bodies external) to preserve the manual-__vftable
    // layout. All three are used in boolean context.
    bool IsFunc() const;
    bool IsProperty(const dsDATA *self, dsSTRID member) const;
    bool CallFunc(dsSTRID method, dsDATA *self, dsDATA *args, int nArgs, dsDATA *ret) const;
} dsDATA_TYPE;
