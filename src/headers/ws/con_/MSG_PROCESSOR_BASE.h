#pragma once
#include "CON_MSG.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../os/osLOCK.h"
// ws-engine con: base class for a console message queue processor — owns a lock-protected
// receive queue of con::CON_MSG plus raw in/out byte buffers for the wire protocol.
// NOTE: this subsystem's prefix is "con", but the directory is "con_" because "con" is a
// Windows-reserved device name that git/Win32 cannot index.
// DB-verified layout (types_members con::MSG_PROCESSOR_BASE) -- size 136.

namespace con {

// Poll-callback slot type for SendImmMsg (typedef'd so the method declaration
// does not read as a function-pointer data member).
typedef int (*MSG_PROC_POLL_CB)();

struct MSG_PROCESSOR_BASE; // forward decl for vtbl fn-ptr signatures

typedef struct MSG_PROCESSOR_BASE_vtbl {
    void (*dtr_MSG_PROCESSOR_BASE)(MSG_PROCESSOR_BASE *self, int deleteFlag);                    // 0x00 — deleting dtor: vftable+0x00 holds ??_GMSG_PROCESSOR_BASE@con@@UAAPAXI@Z
    void (*Disconnect)(MSG_PROCESSOR_BASE *self, const dsTSTRING<char> *reason); // 0x04 // boundary
} MSG_PROCESSOR_BASE_vtbl;

typedef struct MSG_PROCESSOR_BASE {
    MSG_PROCESSOR_BASE_vtbl   *__vftable;    // 0x00
    osLOCK                     lock;         // 0x04 guards receQueue/outBuf/inBuf
    int                        isConnected;  // 0x38
    dsVECTOR<CON_MSG, 8>       receQueue;    // 0x3C pending inbound messages
    dsVECTOR<unsigned char,8>  outBuf;       // 0x50 pending outbound wire bytes
    dsVECTOR<unsigned char,8>  inBuf;        // 0x64 raw inbound wire bytes (not yet framed)
    int                        inBufOfs;     // 0x78 read cursor into inBuf
    unsigned int               pingTime;     // 0x7C
    dsTSTRING<char>            appID;        // 0x80
    dsTSTRING<char>            lastError;    // 0x84

    // True when the receive queue holds at least one non-terminator (id != 3) message;
    // drains any leading terminator entries first. 0x8309B190.
    bool HaveMsg();

    // Pop the front message out of receQueue into `msg` (steals the CON_MSG::data buffer
    // via a field-swap rather than a copy, then Erases the queue slot). 0x8309B268.
    void GetMsg(CON_MSG &msg);

    // Frame `data`/`datalen` with an 8-byte little-endian-swapped {datalen,id} header (or a
    // bare 4-byte length prefix when id==0) and append it to outBuf, throttling until outBuf
    // has room when id is nonzero. 0x8309B380.
    void SendMsg(const void *data, int datalen, int id);

    // Send `data` as an id==2 request via SendMsg, then poll receQueue (spinning on `cb` and
    // logging backoff) until a matching id==3 response or id==4 terminator arrives, returning
    // it via `respond`; discards any stray id==2 entries seen while waiting. Returns 1 on a
    // response, 0 on disconnect/terminator/callback-abort. 0x8309B718.
    int SendImmMsg(const void *data, int datalen, CON_MSG &respond, MSG_PROC_POLL_CB *cb);
} MSG_PROCESSOR_BASE;

} // namespace con
