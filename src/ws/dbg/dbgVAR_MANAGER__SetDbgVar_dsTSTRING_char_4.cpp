#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::SetDbgVar<dsTSTRING<char>,4> @ 0x823b54b8
// Look up the dbgVar named `name`; if found and its typeId is 4 (dbgVAR_STRING's typeId), forward
// to SetDbgVarValue to overwrite its current value with `val`. A type mismatch or absent variable
// is silently ignored.
template<>
void dbgVAR_MANAGER::SetDbgVar<dsTSTRING<char>, 4>(const dsTSTRING<char> &name, const dsTSTRING<char> &val)
{
    dbgVAR *found = this->FindFirst(name);
    if (found && found->typeId == 4)
        this->SetDbgVarValue<dsTSTRING<char>, 4>(found, val);
}
