// HCEX_MODEL_REGION — a named region of a Halo model (a set of object sub-ids/names). POD-ish
// boilerplate: default/copy ctor, dtor, scalar deleting dtor. See
// src/headers/hcex/HCEX_MODEL_REGION.h for the DB-verified layout.

#include "../../headers/hcex/HCEX_MODEL_REGION.h"

// @0x823D67E8 — default-construct: objIds/objNames adopt the ds_vector.h:24 call-site cookie
// (empty vectors), name adopts the shared empty-string singleton via UnsafeInitEmpty().
// objRegId is left uninitialized, matching the disassembly.
HCEX_MODEL_REGION::HCEX_MODEL_REGION()
    : objIds(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
    , objNames(apCL{"D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h", 24})
{
    name.pBuffer = nullptr;
    name.UnsafeInitEmpty();
}

// 0x823E42AC — copy-construct: copy objRegId, deep-copy objIds/objNames via Insert (adopting each
// source vector's call-site cookie), then share that.name's buffer (ref-bump).
HCEX_MODEL_REGION::HCEX_MODEL_REGION(const HCEX_MODEL_REGION &that)
    : objRegId(that.objRegId)
    , objIds(that.objIds.__cl)
    , objNames(that.objNames.__cl)
{
    objIds.Insert(0, that.objIds.pData, that.objIds.nElem);
    objNames.Insert(0, that.objNames.pData, that.objNames.nElem);
    name.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *buffer = that.name.pBuffer;
    name.pBuffer = buffer;
    ++buffer->refCount;
}

// 0x823D4AFC — destructor body is empty: name/objNames/objIds all destruct via the implicit
// per-member teardown (in reverse declaration order: name, objNames, objIds), matching the
// disasm's name-refcount-release, ~dsVECTOR<dsTSTRING<char>,8> call, and objIds.pData dlFree
// sequence. DEVIATION: name's release and objIds's vector-of-POD teardown are fully inlined by the
// compiler (both are tiny), while objNames's vector-of-dsTSTRING teardown remains a visible CALL --
// all three are still just the automatic member destruction, reproduced here without a manual body
// to avoid double-releasing any of them.
HCEX_MODEL_REGION::~HCEX_MODEL_REGION()
{
}

// 0x823D5FE8 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_MODEL_REGION(), then
// conditionally operator-delete `this` when bit 0 of deleteFlags is set.
HCEX_MODEL_REGION *HCEX_MODEL_REGION::ScalarDeletingDtor(unsigned char deleteFlags)
{
    this->~HCEX_MODEL_REGION();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
