#include "dbgVARWrapper.h"
#include <new> // placement new (operator new(size_t, void*)) used below

// Debug-heap placement-new overload used for tagged allocations throughout this build
// (file/line tagged, as seen elsewhere in the corpus, e.g. dsDATA_TYPE_IMPL<double>::ConstructFromObj).
extern void *operator new(size_t size, const char *file, unsigned int line); // size_t: MSVC operator new requires size_t first param
// ds::ToString(dsSTRID) -- ds-subsystem free function; boundary (other-subsystem call).
namespace ds { dsTSTRING<char> ToString(dsSTRID id); }
extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

namespace wb {

// wb::dbgVARWrapper::GetValue() @ 0x8269DDB0 -- lazily create the backing dbgVAR_SIMPLE<bool,1>
// on first call (registering it under this wrapper's display name, seeded from defaultVal),
// then return its live current value.
//
// CAVEAT: the decompile casts the ds::ToString argument as `(dsSTRID*)this->name.id`, i.e. as
// if it reinterprets the interned-string pointer as a dsSTRID*. Disasm (0x8269ddf8 `lwz r4,
// 0(r31)`; the call at 0x8269de00) shows r4 is loaded with `this->name.id` and passed directly
// to ds::ToString(dsSTRID) — dsSTRID is a 4-byte class passed by value in one GPR per this
// ABI, i.e. the real call is `ds::ToString(this->name)` (whole dsSTRID by value), not a pointer
// cast. Reconstructed as the by-value call.
//
// Also faithfully reproduces the disassembly's two-step construction: the freshly allocated
// storage is constructed as a dbgVAR_IMPL<bool,1> (that level's own constructor call, which
// installs the dbgVAR_IMPL vtable), and only afterward is __vftable patched to
// dbgVAR_SIMPLE<bool,1>::vftable (0x8269de14-0x8269de20) -- i.e. this is dbgVAR_SIMPLE<bool,1>'s
// constructor with its base-class step and its own vptr-install step visible separately.
bool dbgVARWrapper::GetValue()
{
    dbgVAR_SIMPLE<bool, 1> *dbgVar = this->dbgVar;

    if (!dbgVar)
    {
        dbgVAR_IMPL<bool, 1> *newVar = (dbgVAR_IMPL<bool, 1> *)operator new(
            sizeof(dbgVAR_SIMPLE<bool, 1>),
            "D:\\Projects\\code\\common\\src.sys\\ap\\ap_whiteboard.cpp", 0x5D);

        if (newVar)
        {
            dsTSTRING<char> displayName = ds::ToString(this->name);
            new (newVar) dbgVAR_IMPL<bool, 1>(displayName, this->defaultVal);
            newVar->__vftable = (dbgVAR_vtbl *)&dbgVAR_SIMPLE<bool, 1>::vftable;
            dbgVar = (dbgVAR_SIMPLE<bool, 1> *)newVar;

            dsTSTRING_BUF_HEADER<char> *nameBuffer = displayName.pBuffer;
            if (--nameBuffer->refCount == 0)
                dlFree(nameBuffer);
        }
        else
        {
            dbgVar = nullptr;
        }

        this->dbgVar = dbgVar;
    }

    return this->dbgVar->value;
}

} // namespace wb
