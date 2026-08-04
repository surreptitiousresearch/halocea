#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::AddVar @ 0x82560488
// Register a newly-constructed debug variable: append it to both the live list and the
// not-yet-synced-to-console list, and flag the console page for a refresh.
// NOTE: the decompiler fabricated phantom int parameters (a3..a14) from stray stack slots past
// the real argument -- the disasm confirms the only real arguments are `this` (r3) and `var` (r4),
// matching the mangled signature (?AddVar@dbgVAR_MANAGER@@QAAXPAVdbgVAR@@@Z).
void dbgVAR_MANAGER::AddVar(dbgVAR *var)
{
    this->lock.Lock(nullptr, 0);
    this->vars.PushBack(var);
    this->newVars.PushBack(var);
    this->needUpdateConsole = true;
    this->lock.Unlock(nullptr, 0);
}
