#include "apLOG_CON_CB.h"

// --- Boundaries (con message protocol / file I/O / os time). MSG_PROTOCOL and fioFILE layouts
// are not reversed here; modeled as opaque handles passed through free-function shims. ---
namespace con { struct MSG_PROTOCOL; }
struct fioFILE;

extern con::MSG_PROTOCOL *apConsole;                 // boundary global — the ap debug console
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// DEVIATION: slot 3 was `unsigned char reliable` fed by an invented `g_conStartMsgReliable` global.
// ?StartMsg@MSG_PROTOCOL@con@@QAAAAVfioFILE@@PBD00@Z is (char const*, char const*, char const*), and
// 0x82AEF9F0/0x82AEF9FC load `&byte_8200155A` into r5 — the shared "" literal, not a bool.
extern fioFILE *MSG_PROTOCOL_StartMsg(con::MSG_PROTOCOL *console, const char *page, const char *msgName,
                            const char *tag);          // con::MSG_PROTOCOL::StartMsg
extern void MSG_PROTOCOL_SendMsg(con::MSG_PROTOCOL *console, fioFILE *msg);   // con::MSG_PROTOCOL::SendMsg
extern void MSG_PROTOCOL_CancelMsg(con::MSG_PROTOCOL *console, fioFILE *msg); // con::MSG_PROTOCOL::CancelMsg
extern void fioFILE_WriteData(fioFILE *file, const void *data, int size, int count); // fioFILE::WriteData
extern "C" unsigned int osGetTime();

// apLOG_CON_CB::SendAllLog @ 0x82AEF9D8
// Flush the whole queued log to the console page as one reliable message, then reset the queue.
void apLOG_CON_CB::SendAllLog()
{
    fioFILE *msg = MSG_PROTOCOL_StartMsg(apConsole, this->pageName.pBuffer->str, empty_string, "Log");

    int queuedBytes = this->queue_pos;
    fioFILE_WriteData(msg, &queuedBytes, 4, 4);
    fioFILE_WriteData(msg, this->queue.pData, this->queue_pos, 1);

    if (this->pageVersionValid)
        MSG_PROTOCOL_SendMsg(apConsole, msg);
    else
        MSG_PROTOCOL_CancelMsg(apConsole, msg);

    this->lastSend = osGetTime();
    this->queue_pos = 0;
}
