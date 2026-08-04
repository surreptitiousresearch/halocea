#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/fio/fioFILE.h"

// aiCON_CB::SendWatcheeToConsole @ 0x83181558
// ?SendWatcheeToConsole@aiCON_CB@@QAA_N_N@Z
//
// If the debug watchee changed (or `force`), adopt _pBrainDebugWatchee as the current watchee and
// push a "WatcheeInfo" message describing it: the instance name (with its SSL class appended), the
// template name, and a "have watchee" flag. When no brain is watched, the placeholder texts are
// sent with the flag clear.
//
// DEVIATION: the disassembly inlines every dsTSTRING ctor/assign/dtor with explicit buffer
// refcount surgery. Reproduced here with dsTSTRING value semantics (operator=/+/+=) — semantically
// identical, and far less error-prone than transcribing the inlined refcount bookkeeping.
bool aiCON_CB::SendWatcheeToConsole(bool force)
{
    if (!this->IsConnected())
        return false;

    ds::WEAK_PTR_HANDLE<aiBRAIN> *pHandle = this->pCurrentWatchee.pHandle;
    aiBRAIN *pPtr = pHandle ? pHandle->pPtr : nullptr;
    int hasWatchee = 0;

    if (pPtr != _pBrainDebugWatchee || force)
    {
        this->pCurrentWatchee.SetPtr(_pBrainDebugWatchee);

        dsTSTRING<char> classText;                 // v34 — SSL class name (empty by default)
        dsTSTRING<char> nameText;                  // v36 — instance name / payload
        nameText = strNoWatchee_1;
        dsTSTRING<char> tplText("No watchee Tpl"); // v37 — template name

        aiBRAIN *watchee = this->pCurrentWatchee.pHandle ? this->pCurrentWatchee.pHandle->pPtr
                                                         : nullptr;
        if (this->pCurrentWatchee.pHandle && watchee)
        {
            const char *name = watchee->pInst->name;
            if (!name) name = g_emptyString_8200155A;
            nameText = dsTSTRING<char>(name);

            const char *nameClass = watchee->pInst->nameClass;
            if (!nameClass) nameClass = g_emptyString_8200155A;
            classText = dsTSTRING<char>(nameClass);

            const char *nameTpl = watchee->pInst->nameTpl;
            if (!nameTpl) nameTpl = g_emptyString_8200155A;
            tplText = dsTSTRING<char>(nameTpl);

            nameText += (dsTSTRING<char>("   [SSL class = '") + classText) + ".cls']";
            hasWatchee = 1;
        }

        fioFILE *started = &apConsole->StartMsg(this->pageName.pBuffer->str,
                                                this->msgCtrlName.pBuffer->str,
                                                sn_WatcheeInfo_1.pBuffer->str);
        int nameLen = nameText.pBuffer->strLen + 1;
        started->WriteData(&nameLen, 4, 4);
        started->WriteData(nameText.pBuffer->str, nameLen, 1);

        int tplLen = tplText.pBuffer->strLen + 1;
        started->WriteData(&tplLen, 4, 4);
        started->WriteData(tplText.pBuffer->str, tplLen, 1);

        started->WriteData(&hasWatchee, 4, 4);

        if (this->pageVersionValid)
            apConsole->SendMsg(*started);
        else
            apConsole->CancelMsg(*started);
    }

    return true;
}
