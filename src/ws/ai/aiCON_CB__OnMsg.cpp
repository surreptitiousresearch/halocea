#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/fio/fioFILE.h"

// aiCON_CB::OnMsg @ 0x83181FA8  (virtual override of con::CONSOLE_CALLBACK::OnMsg)
// Handle an inbound AI-console-page request: list/line selectors, SSL-vars mode switch, and the
// watchee fast-peek / select-next / clear / kill controls. Returns 1 when the ctrl+msg pair was
// recognised and consumed, 0 otherwise.
int aiCON_CB::OnMsg(const dsTSTRING<char> &ctrl, const dsTSTRING<char> &msg, fioFILE &data,
                    fioFILE *response)
{
    (void)response;
    if (!(ctrl == this->msgCtrlName))
        return 0;

    if (msg == pgReq_ListSelector_1)
    {
        aiINFO_LIST_TYPE sel;
        data.ReadData(&sel, 4, 4);
        this->infoSelectorExtra = sel;
        return 1;
    }
    if (msg == pgReq_LineSelector_1)
    {
        int sel;
        data.ReadData(&sel, 4, 4);
        this->infoSelectorMain.val = sel;
        return 1;
    }
    if (msg == pgReq_SSLVars_1)
    {
        ai::con_cb::INFO_LIST_TYPE requested;
        data.ReadData(&requested, 4, 4);
        this->nextInfoListType = requested;
        // Apply immediately unless a prior SSL-vars request is still pending, and only for a
        // real (non-sentinel) mode. (64-bit compare @ 0x831820A8: hi=haveSslVarsReq, lo=mode.)
        if (!this->haveSslVarsReq && requested != ai::con_cb::ilMAX_VALUE)
        {
            this->infoListType = requested;
            this->nextInfoListType = ai::con_cb::ilMAX_VALUE;
        }
        return 1;
    }
    if (msg == pgReq_WatcheeFastPeek_1)
    {
        int on;
        data.ReadData(&on, 4, 4);
        this->wSelectFastPeek = (on != 0);
        return 1;
    }
    if (msg == pgReq_WatcheeSelectNext_1)
    {
        int live;
        data.ReadData(&live, 4, 4);
        if (live)
            this->WSelectNextLive();
        else
            this->WSelectNextDead();
        this->SendWatcheeToConsole(false);
        return 1;
    }
    if (msg == pgReq_WatcheeClear_1)
    {
        _pBrainDebugWatchee = nullptr;
        this->SendWatcheeToConsole(false);
        return 1;
    }
    if (!(msg == pgReq_WatcheeKill_1))
        return 0;

    if (_pBrainDebugWatchee)
        aiBRAIN_Die(_pBrainDebugWatchee, dsTSTRING<char>("RAGDOLL"));
    return 1;
}
