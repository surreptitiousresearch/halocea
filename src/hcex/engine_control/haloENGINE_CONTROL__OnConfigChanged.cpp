#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::OnConfigChanged @ 0x823C89C8 — gsUserConfigChanged event handler: re-applies
// settings for whichever user slot (primary/secondary) the signaled userID matches.
//
// DEVIATION: the decompiler additionally stores `v6->id.id` into a dead local (`v4[1]`) that is
// never read again before the function returns — a stack-slot-reuse artifact of the same kind
// documented in ApplySettings.cpp's CAVEAT, not an observable side effect. Omitted here.
void haloENGINE_CONTROL::OnConfigChanged(unsigned long eventUserId, const dsPARAM_LIST &paramList)
{
    (void)eventUserId;

    static dsSTRID strIdUserID;
    static bool strIdUserIDInitialized = false;
    if (!strIdUserIDInitialized)
    {
        strIdUserIDInitialized = true;
        strIdUserID = dsSTRID("userID", false);
    }

    const dsPARAM *userIdParam = paramList.Find(strIdUserID);

    int changedUserId = 0;
    bool isRet = userIdParam->data.GetValue<int>(changedUserId, DSD_CONV_RETRIEVE);
    if (!IGNORE_STRONG_ASSERT && !isRet)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("isRet", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_param.h", 28, "");

    ApplySettings(changedUserId == gsUserMng->PrimaryUserIdx());
}
