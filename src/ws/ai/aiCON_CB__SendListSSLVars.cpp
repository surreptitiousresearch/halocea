#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/fio/fioFILE.h"

// aiCON_CB::SendListSSLVars @ 0x831823B0
// Rebuild the SSL-var list text and, when it changed (or forced), push it (mode + length-
// prefixed text) to the AI console page. Applies any pending list-type mode switch.
bool aiCON_CB::SendListSSLVars(bool force)
{
    if (!this->IsConnected())
        return false;

    // Original pre-reserves a 5120-byte buffer (AllocBuffer) as an optimization; GetAISSLVars
    // overwrites `text` wholesale, so the reservation is not observable and is omitted here.
    dsTSTRING<char> text;
    this->GetAISSLVars(text);

    ai::con_cb::INFO_LIST_TYPE pending = this->nextInfoListType;
    this->haveSslVarsReq = false;
    if (pending != ai::con_cb::ilMAX_VALUE)
    {
        this->infoListType = pending;
        this->nextInfoListType = ai::con_cb::ilMAX_VALUE;
    }

    if (force || !(text == this->aiListSSLVars))
    {
        this->aiListSSLVars = text;

        fioFILE *msg = &apConsole->StartMsg(this->pageName.pBuffer->str,
                                            this->msgCtrlName.pBuffer->str,
                                            sn_SSLVars_1.pBuffer->str);
        int mode = this->infoListType;
        msg->WriteData(&mode, 4, 4);
        int len = this->aiListSSLVars.pBuffer->strLen + 1;
        msg->WriteData(&len, 4, 4);
        msg->WriteData(this->aiListSSLVars.pBuffer->str, len, 1);

        if (this->pageVersionValid)
            apConsole->SendMsg(*msg);
        else
            apConsole->CancelMsg(*msg);
    }
    return true;
}
