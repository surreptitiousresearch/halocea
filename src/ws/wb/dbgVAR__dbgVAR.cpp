#include "dbgVAR_boundary.h"
#include "../dbg/dbgVAR_MANAGER.h"
#include "../ds/ds_assert_boundary.h"

// dbgVAR::dbgVAR(const dsTSTRING<char> &_name, int _typeId) @ 0x82561658
// Base constructor shared by every dbgVAR_IMPL<T,N>/dbgVAR_SIMPLE<T,N> leaf: installs this level's
// vtable, shares `_name`'s buffer (manual refcount bump -- the compiler-inlined equivalent of
// dsTSTRING<char>'s copy-share), and records `_typeId`. It then guards against re-registering the
// same display name under a different type (a STRONG_ASSERT crash, bypassed when
// IGNORE_STRONG_ASSERT is set) before registering itself with the process-wide dbgVAR_MANAGER.
// NOTE: the decompiler fabricated phantom trailing int parameters (v10..v21) on the AddVar call
// from stray stack slots past the real argument -- the disasm confirms AddVar's only real
// arguments are the manager (r3) and `this` (r4), matching dbgVAR_MANAGER::AddVar's mangled
// signature.
dbgVAR::dbgVAR(const dsTSTRING<char> &_name, int _typeId)
    : __vftable(&dbgVAR::vftable)
    , typeId(_typeId)
{
    this->name.pBuffer = nullptr;
    this->name.pBuffer = _name.pBuffer;
    ++this->name.pBuffer->refCount;

    dbgVAR_MANAGER *manager = dbgVAR_MANAGER::GetManager();
    dbgVAR *existing = manager->FindFirst(_name);
    if (existing && _typeId != existing->typeId && !IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "0",
            "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dbg_var.cpp",
            533,
            empty_string);

    dbgVAR_MANAGER::GetManager()->AddVar(this);
}
