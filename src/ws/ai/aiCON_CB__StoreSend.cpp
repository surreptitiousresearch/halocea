#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/fio/fioFILE.h"

extern "C" void dlFree(void *ptr); // engine heap free (boundary)

// aiCON_CB::StoreSend @ 0x83180D20
// ?StoreSend@aiCON_CB@@IAAX_NABV?$dsTSTRING@D@@AAV2@PBD@Z
//
// Diff the freshly-built list text `fresh` against the last-sent `placed`: when it changed (or the
// caller forces it), adopt the new buffer into `placed` and push it to the AI console page as a
// message named `msg`. The message payload is the string length (+1) followed by the NUL-terminated
// text. `pageVersionValid` decides whether the framed message is actually sent or cancelled.
void aiCON_CB::StoreSend(bool force, const dsTSTRING<char> &fresh, dsTSTRING<char> &placed,
                         const char *msg)
{
    if (force || !(fresh == placed))
    {
        // Inlined dsTSTRING assignment (placed = fresh): release the old buffer, adopt fresh's.
        if (&fresh != &placed)
        {
            dsTSTRING_BUF_HEADER<char> *oldBuf = placed.pBuffer;
            if (--placed.pBuffer->refCount == 0)
                dlFree(oldBuf);
            placed.pBuffer = fresh.pBuffer;
            ++placed.pBuffer->refCount;
        }

        fioFILE *started = &apConsole->StartMsg(this->pageName.pBuffer->str,
                                                this->msgCtrlName.pBuffer->str, msg);
        int payloadLen = fresh.pBuffer->strLen + 1;
        started->WriteData(&payloadLen, 4, 4);
        started->WriteData(fresh.pBuffer->str, fresh.pBuffer->strLen + 1, 1);
        if (this->pageVersionValid)
            apConsole->SendMsg(*started);
        else
            apConsole->CancelMsg(*started);
    }
}
