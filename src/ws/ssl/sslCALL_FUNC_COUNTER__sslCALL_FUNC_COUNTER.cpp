#include "sslCALL_FUNC_COUNTER.h"
#include "sslSYSTEM.h"

// sslCALL_FUNC_COUNTER::sslCALL_FUNC_COUNTER(sslSYSTEM *ssl) @ 0x83087B68
// Bind the owning system and bump the process-wide call-nesting counter. When the debugger has
// requested a single-step (dbgMode == 2, i.e. "step into the next call"), arm the trap (dbgMode
// -> 3) and latch `Skip` to the post-increment nesting depth, so the matching destructor knows
// which depth to stop stepping at.
int sslCALL_FUNC_COUNTER::Count = 0;
int sslCALL_FUNC_COUNTER::Skip  = 0;

sslCALL_FUNC_COUNTER::sslCALL_FUNC_COUNTER(sslSYSTEM *ssl)
{
    this->Ssl = ssl;
    ++sslCALL_FUNC_COUNTER::Count;
    if (this->Ssl->dbgMode == 2)
    {
        this->Ssl->dbgMode = 3;
        sslCALL_FUNC_COUNTER::Skip = sslCALL_FUNC_COUNTER::Count;
    }
}
