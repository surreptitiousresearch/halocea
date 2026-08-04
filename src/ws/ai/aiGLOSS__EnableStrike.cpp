#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiGLOSS_local.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// 0x8323AB20 — aiGLOSS::EnableStrike. Toggle whether the strike sound `name` is in the disabled
// list: disabling adds it (logging if already disabled); enabling removes it (logging if it was
// not disabled in the first place).
void aiGLOSS::EnableStrike(const dsTSTRING<char> &name, bool isEnable)
{
    int idx = FindInVector(&strikeDisabled, &name);

    if ( !isEnable )
    {
        if ( idx == -1 )
        {
            strikeDisabled.PushBack(name);
            return;
        }
        // Already disabled — DEVIATION: the original _apLog call is a garbled PPC vararg sequence;
        // reconstructed to the single diagnostic the branch was building.
        _apLog("~AI,Ssl~AI: attempt to disable strike('%s') that is already disabled", name.pBuffer->str);
        return;
    }

    if ( idx == -1 )
    {
        _apLog("~AI,Ssl~AI: attempt to disable strike('%s') that is already disabled", name.pBuffer->str);
        return;
    }
    strikeDisabled.Erase(idx, 1);
}
