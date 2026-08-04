#pragma once
#include "../con_/CONSOLE_CALLBACK.h"
#include "../ds/dsVECTOR.h"
// ws-engine ap (platform): console page that buffers log text and periodically flushes the
// whole queue to the debug console. Derives con::CONSOLE_CALLBACK.
// DB-verified layout (types_members apLOG_CON_CB) — size 48.

struct apLOG_CON_CB : con::CONSOLE_CALLBACK {
    dsVECTOR<char, 8> queue;     // 0x10 accumulated log bytes
    int               queue_pos; // 0x24 bytes currently queued
    unsigned int      lastSend;  // 0x28 time of last flush
    unsigned int      timeStart; // 0x2C

    // 0x82AEFCA0 — format one log line ("#<seconds.millis>#" + `message`) and append it to the
    // queue, flushing the whole queue to the console when it would overflow or 300 ms have elapsed
    // since the last send. Public (QAA mangle).
    void AddLogMessage(const char *message);

    // 0x82AEF9D8 — flush the entire queued log to the console page as one message, then reset
    // the queue. Protected (IAA mangle).
protected:
    void SendAllLog();
};
