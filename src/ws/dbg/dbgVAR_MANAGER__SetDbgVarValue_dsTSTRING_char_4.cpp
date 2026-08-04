#include "dbgVAR_MANAGER.h"
#include "../os/os_boundary.h" // dlFree

// dbgVAR_MANAGER::SetDbgVarValue<dsTSTRING<char>,4> @ 0x823b51b0
// Protected helper behind SetDbgVar<dsTSTRING<char>,4>: overwrite the string dbgVar's current
// value by sharing `val`'s buffer (manual dsTSTRING buffer release/share -- the compiler-inlined
// equivalent of dsTSTRING<char>::operator=), then notify the console page via SendVarUpdate.
// NOTE: the decompiler mistyped `var` as `dsDATA *` and `val` as `dbgVAR *`, and rendered the
// buffer-header refcount field access as `->__vftable` -- the mangled name
// (?SetDbgVarValue@dbgVAR_MANAGER@@AAAXPAVdbgVAR@@ABV?$dsTSTRING@D@@@Z) and the disasm confirm the
// real signature is (dbgVAR *var, const dsTSTRING<char> &val); `&var[1].type` is `var + 0xC`, i.e.
// the address of `value` within the dbgVAR_IMPL<dsTSTRING<char>,4> that `var` actually points to
// (dbgVAR's own size is 12 bytes), and the "__vftable" reads/writes are really the string's
// dsTSTRING_BUF_HEADER<char>* refcounted buffer pointer.
template<>
void dbgVAR_MANAGER::SetDbgVarValue<dsTSTRING<char>, 4>(dbgVAR *var, const dsTSTRING<char> &val)
{
    dbgVAR_IMPL<dsTSTRING<char>, 4> *stringVar = static_cast<dbgVAR_IMPL<dsTSTRING<char>, 4> *>(var);

    if (&val != &stringVar->value)
    {
        dsTSTRING_BUF_HEADER<char> *oldBuffer = stringVar->value.pBuffer;
        if (--oldBuffer->refCount == 0)
            dlFree(oldBuffer);

        dsTSTRING_BUF_HEADER<char> *newBuffer = val.pBuffer;
        stringVar->value.pBuffer = newBuffer;
        ++newBuffer->refCount;
    }

    this->SendVarUpdate(var);
}
