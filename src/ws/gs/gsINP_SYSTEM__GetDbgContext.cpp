#include "../../headers/ws/gs/gsINP_SYSTEM.h"

// 0x826565A0 — return this system's debug binding-context handle (this->dbgCtx @ 0x40) by value,
// bumping its backing refcount. Effectively a copy of the handle with an incremented reference.
// DEVIATION: the raw decompile is garbled — it transposed the sret result pointer (r3) and the
// `this` pointer (r4) and rendered the copy as writes to `this->__vftable` / `this->dtr_...`.
// The disassembly is unambiguous: r10 = *(this + 0x40) (dbgCtx.m_pData) is stored into *result,
// then result->m_pData->m_iRefCount (offset +4) is incremented, and r3 (result) is returned.
// The backing cell is dereferenced without a null check, matching the original.
ds_data::REF_TYPE<gsiCONTEXT> gsINP_SYSTEM::GetDbgContext()
{
    ds_data::REF_TYPE<gsiCONTEXT> result;
    result.m_pData = dbgCtx.m_pData;
    ++result.m_pData->m_iRefCount;
    return result;
}
