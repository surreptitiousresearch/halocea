// apLOG_CON_CB::AddLogMessage @ 0x82AEFCA0 — public (QAA mangle).
// Build one log line of the form "#<seconds.millis>#<message>" and append it to the byte queue.
// If the queue would overflow or 300 ms have elapsed since the last send, flush the whole queue to
// the console page first (under lockConSend) and emit this line as its own message instead.
//
// CAVEAT: the binary inlines dsTSTRING's private gap-open (Insert(pos,len)) followed by direct
// buffer writes to build the line. That protected primitive is not callable from here, so the same
// character-level edits are expressed through the public dsTSTRING::Insert overloads (they are
// semantically identical: open a gap and fill it). The time value is formatted with itoa into a
// Lock()ed buffer whose length is then recomputed, exactly as the original does. The two temporary
// strings are torn down with explicit refcount drops, matching the binary (dsTSTRING has no
// user-declared destructor in these headers).
#include "apLOG_CON_CB.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include "../os/osLOCK.h"
#include <string.h>

extern "C" unsigned int osGetTime();                       // boundary — millisecond clock
extern "C" char *itoa(int value, char *str, int radix);    // boundary — CRT integer->string
extern "C" void dlFree(void *ptr);                         // boundary — dlmalloc free

// --- con message protocol / file I/O boundaries (layouts not reversed; see SendAllLog.cpp) ---
namespace con { struct MSG_PROTOCOL; }
struct fioFILE;

extern con::MSG_PROTOCOL *apConsole;                // boundary global — the ap debug console
extern osLOCK lockConSend;                          // boundary global — send serialization lock
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// DEVIATION: slot 3 was `unsigned char reliable` fed by an invented `g_conStartMsgReliable` global.
// ?StartMsg@MSG_PROTOCOL@con@@QAAAAVfioFILE@@PBD00@Z is (char const*, char const*, char const*), and
// the caller loads `&byte_8200155A` into r5 — the shared "" literal, not a bool. See SendAllLog.cpp.
extern fioFILE *MSG_PROTOCOL_StartMsg(con::MSG_PROTOCOL *console, const char *page, const char *msgName,
                            const char *tag);        // con::MSG_PROTOCOL::StartMsg
extern void MSG_PROTOCOL_SendMsg(con::MSG_PROTOCOL *console, fioFILE *msg);   // con::MSG_PROTOCOL::SendMsg
extern void MSG_PROTOCOL_CancelMsg(con::MSG_PROTOCOL *console, fioFILE *msg); // con::MSG_PROTOCOL::CancelMsg
extern void fioFILE_WriteData(fioFILE *file, const void *data, int size, int count); // fioFILE::WriteData

void apLOG_CON_CB::AddLogMessage(const char *message)
{
    int messageLen = (int)strlen(message);

    // line = "#"
    dsTSTRING<char> line;
    line.pBuffer = nullptr;
    line.UnsafeInitEmpty();
    line.Insert(line.pBuffer->strLen, "#", -1);

    // timeText = milliseconds since this page started, as a decimal string
    dsTSTRING<char> timeText;
    timeText.pBuffer = nullptr;
    timeText.UnsafeInitEmpty();

    int elapsedMs = (int)(osGetTime() - this->timeStart);
    char *timeBuf = timeText.Lock(32);
    itoa(elapsedMs, timeBuf, 10);

    // Lock() left strLen at the reserved size; recompute it from the itoa NUL terminator.
    dsTSTRING_BUF_HEADER<char> *timeHeader = timeText.pBuffer;
    int capacity = timeHeader->maxStrLen;
    int measured = 0;
    if (capacity >= 0)
    {
        while (timeHeader->str[measured] && measured <= capacity)
            ++measured;
    }
    if (capacity < 0 || measured > capacity)
    {
        timeHeader->strLen = capacity;
        timeHeader->str[capacity] = 0;
    }
    else
    {
        timeHeader->strLen = measured;
    }

    // insert a decimal point three digits from the right (ms -> seconds.millis)
    if (timeText.pBuffer->strLen > 3)
        timeText.Insert(timeText.pBuffer->strLen - 3, (char)'.', 1);

    // line = "#" + timeText + "#"
    line.Insert(line.pBuffer->strLen, timeText.CStr(), timeText.pBuffer->strLen);
    line.Insert(line.pBuffer->strLen, (char)'#', 1);
    dsTSTRING_BUF_HEADER<char> *lineHeader = line.pBuffer;
    int lineLen = lineHeader->strLen;

    if (lineLen + messageLen + this->queue_pos >= this->queue.nElem
        || osGetTime() - this->lastSend > 300)
    {
        // flush path: drain the queue, then send this line as a standalone console message
        lockConSend.Lock(nullptr, 0);
        this->SendAllLog();

        fioFILE *msg = MSG_PROTOCOL_StartMsg(apConsole, this->pageName.pBuffer->str,
                                   empty_string, "Log");
        int total = lineLen + messageLen + 1;
        fioFILE_WriteData(msg, &total, 4, 4);
        fioFILE_WriteData(msg, lineHeader->str, lineLen, 1);
        fioFILE_WriteData(msg, message, messageLen + 1, 1);

        if (this->pageVersionValid)
            MSG_PROTOCOL_SendMsg(apConsole, msg);
        else
            MSG_PROTOCOL_CancelMsg(apConsole, msg);

        lockConSend.Unlock(nullptr, 0);
    }
    else
    {
        // buffer path: append the formatted line, then the message (including its NUL)
        strncpy(&this->queue.pData[this->queue_pos], lineHeader->str, lineLen);
        this->queue_pos += lineLen;
        strcpy(&this->queue.pData[this->queue_pos], message);
        this->queue_pos += messageLen + 1;
    }

    // tear down the two temporary strings
    dsTSTRING_BUF_HEADER<char> *timeBuffer = timeText.pBuffer;
    if (timeBuffer->refCount-- == 1)
        dlFree(timeBuffer);
    if (lineHeader->refCount-- == 1)
        dlFree(lineHeader);
}
