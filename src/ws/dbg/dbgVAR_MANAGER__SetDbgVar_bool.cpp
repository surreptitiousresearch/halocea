#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::SetDbgVar<bool,1> @ 0x823C4210
// Look up the dbgVar named `name`; if found and its typeId is 1 (dbgVAR_BOOL's typeId), overwrite
// its current value with `val` and notify the console page via SendVarUpdate. A type mismatch or
// absent variable is silently ignored.
// Unlike the dsTSTRING specialisation there is no separate protected SetDbgVarValue<bool,1> in this
// binary -- the value write is inlined here. The decompiler rendered the store as
// `HIBYTE(First[1].__vftable) = *val`: First is a dbgVAR* (size 12), so First[1] is at +12, and on
// this big-endian PPC target HIBYTE of the 32-bit word at +12 is the byte at +12 -- i.e.
// dbgVAR_IMPL<bool,1>::value (offset 0x0C).
template<>
void dbgVAR_MANAGER::SetDbgVar<bool, 1>(const dsTSTRING<char> &name, const bool &val)
{
    dbgVAR *found = this->FindFirst(name);
    if (found && found->typeId == 1)
    {
        static_cast<dbgVAR_IMPL<bool, 1> *>(found)->value = val;
        this->SendVarUpdate(found);
    }
}
