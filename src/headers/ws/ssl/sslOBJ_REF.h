#pragma once
#include "../ds/dsTSTRING.h"
// ssl subsystem: script object reference. DB-verified layout (types_members sslOBJ_REF):
//   pObject@0 (sslOBJECT *) — size 4. Method bodies that dereference pObject live in the
//   .cpp (which includes sslOBJECT.h); this header only forward-declares sslOBJECT.

struct sslOBJECT;   // ssl script object instance (full definition in sslOBJECT.h)
struct sslCB_HOST;  // ssl callback host (full definition in sslCB_HOST.h)
struct sslCLASS_REF; // ref to a script class descriptor (full definition in sslCLASS_REF.h)
struct sslERROR;    // ssl error/result value (full definition in sslERROR.h)
struct sslSYSTEM;   // owning script system (full definition in sslSYSTEM.h)
struct dsDATA;      // ds type-erased value (full definition in ds/dsDATA.h)
struct dsSTRID;     // ds interned string id (full definition in ds/dsSTRID.h)

typedef struct sslOBJ_REF {
    sslOBJECT *pObject; // 0x00 referenced object (null when empty)

    sslOBJ_REF();                                   // 0x82523220 — empty ref (pObject = null)
    sslOBJ_REF(const sslOBJ_REF &other);            // 0x82523A90 — ref-counted copy
    sslOBJ_REF(sslOBJECT *pObj);                    // 0x82523B00 — adopt a raw object, ref-counted
    ~sslOBJ_REF();                                  // 0x825236C8 — releases the object ref
    sslOBJ_REF &operator=(const sslOBJ_REF &other); // 0x82523B70 — ref-counted assign

    // 0x825242D0 — convert to a type-erased dsDATA by value (sret): zeroes the dest's type field,
    // then stores this ref into it via dsDATA::StoreValue<sslOBJ_REF> (adopts the sslOBJ_REF
    // dsDATA_TYPE descriptor and copy-constructs the value). const (QBA).
    operator dsDATA() const;

    // Resolve the referenced object and return its name as a fresh dsTSTRING<char> (by value);
    // body not decompiled here. Used by dsVECTOR<sslOBJ_REF,8>::FindSorted<sslCMP_NAMES,...>.
    dsTSTRING<char> GetName() const; // boundary

    // 0x82523388 — uid of the referenced object, or -1 if empty. const (QBA).
    int GetUID() const;

    // ?SetDbgName@sslOBJ_REF@@QBAXABV?$dsTSTRING@D@@@Z — forward a debug name onto the referenced
    // object (sslOBJECT::SetDbgName) for script diagnostics. const (QBA). boundary.
    void SetDbgName(const dsTSTRING<char> &name) const;

    // ?GetCbHost@sslOBJ_REF@@QBAPAVsslCB_HOST@@XZ — the callback host of the referenced object
    // (its sslOBJECT::GetCbHost), or null when empty. const (QBA). Consumers cast the result to
    // the concrete host type (e.g. iaIACTOR*).
    sslCB_HOST *GetCbHost() const;

    // 0x825261F0 — debug name of the referenced object (returns a fresh dsTSTRING<char> by value);
    // yields the sentinel "<<nullobj>>" when the ref is empty. const (QBA).
    dsTSTRING<char> GetDbgName() const;

    // 0x82524370 — the class of the referenced object (returns a fresh sslCLASS_REF by value);
    // an empty ref when this ref is empty. const (QBA).
    sslCLASS_REF GetClass() const;

    // 0x82523728 — retarget pObject to `pObj` with ref-count fixup (adds a ref to the new object,
    // releases and destroys the old one when its count hits zero). QAA.
    void _SetObjectPtr(sslOBJECT *pObj);

    // 0x82523BE0 — dispatch function/callback `idx` on the referenced object (via its class), on
    // behalf of `caller`; returns an sslERROR by value (sret). Yields a formatted error when this
    // ref is empty (the object was never parsed). QAA. `caller` is passed by value (its ref is
    // released on return).
    sslERROR CallFunc(int idx, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);

    // 0x825240C0 — retrieve the value of member (sub-element) `idx` on the referenced object into
    // `out`; returns an sslERROR by value (SSL_ERR_NO_ERROR on success). QAA. boundary.
    sslERROR GetVar(int idx, dsDATA &out);

    // 0x82523D88 — dispatch event `idx` on the referenced object (via its class) with (argc, argv);
    // returns an sslERROR by value (sret). Yields a formatted SSL_ERR_GENERIC error when this ref is
    // empty (the object is <null>). QAA. `argv` is an array of dsDATA pointers.
    sslERROR HandleEvent(int idx, int argc, dsDATA *const *argv);

    // 0x8252xxxx — dispatch event `idx` on the referenced object with no arguments (argc==0). Thin
    // overload of the above; returns an sslERROR by value (sret). QAA. boundary.
    // (mangled _HandleEvent_sslOBJ_REF__QAA_AVsslERROR__H_Z — single int arg)
    sslERROR HandleEvent(int idx);

    // dispatch event `idx` on the referenced object passing a single event payload `event`; returns
    // an sslERROR by value (sret). QAA. boundary.
    // (mangled _HandleEvent_sslOBJ_REF__QAA_AVsslERROR__HABVdsDATA___Z — int + dsDATA const&)
    sslERROR HandleEvent(int idx, const dsDATA &event);

    // 0x82526578 — resolve function `name` on the referenced object's class to an element index and
    // dispatch it (overload __3 of CallFunc). Yields an SSL_ERR_UNKNOWN_NAME error when the name is
    // not found, or an SSL_ERR_GENERIC error when the ref is empty. Returns sslERROR by value (sret).
    // QAA. `caller` is passed by value (its ref is released on return).
    sslERROR CallFunc(dsSTRID name, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);

    // 0x82524AC0 — retarget this ref from a type-erased dsDATA value: copy `data`, convert it to an
    // sslOBJ_REF, and (on success) install it into this ref. Returns sslERROR by value (sret): a
    // no-error result on success, else an SSL_ERR_RUNTIME conversion error. `pSsl` is accepted for
    // signature parity. QAA.
    sslERROR Set(const dsDATA &data, sslSYSTEM *pSsl);

    // 0x825237D8 (?AllowDelayedFuncs@sslOBJ_REF@@QAAXH@Z) — enable/disable queuing of deferred
    // script-function calls on the referenced object. boundary.
    void AllowDelayedFuncs(int allow);

} sslOBJ_REF;
