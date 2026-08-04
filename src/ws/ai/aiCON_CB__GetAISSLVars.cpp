#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"

// aiCON_CB::GetAISSLVars @ 0x83182268
// Produce the SSL-var list text for the currently-selected watchee into `list`, dispatched by
// the active INFO_LIST_TYPE mode. Returns false only for an out-of-range mode.
bool aiCON_CB::GetAISSLVars(dsTSTRING<char> &list)
{
    ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = this->pCurrentWatchee.pHandle;
    if (!handle || !handle->pPtr)
    {
        list = strNoWatchee_1;
        return true;
    }

    ai::con_cb::INFO_LIST_TYPE mode = this->infoListType;
    if ((unsigned int)mode > (unsigned int)ai::con_cb::ilStrikes)
        return false;

    if (mode == ai::con_cb::ilSslBrain)
    {
        this->GetAISSLVarsBrain(list);
        return true;
    }
    if (mode == ai::con_cb::ilSslSquad)
    {
        this->GetAISSLVarsSquad(list);
        return true;
    }
    if (mode != ai::con_cb::ilOFF)
    {
        this->AddLineStrikes(list);
        return true;
    }

    list = dsTSTRING<char>("Disabled");
    return true;
}
