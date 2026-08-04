#pragma once
#include "MSG_PROCESSOR.h"
#include "../os/osLOCK.h"
#include "../ds/dsTSTRING.h"
#include "../fio/fioFILE.h"
#include "../fio/fioFILE_MEM.h"
#include "../ds/dsVECTOR.h"
// ws-engine con: the console wire-protocol layer that drives a con::MSG_PROCESSOR backend --
// starts/sends/cancels console messages framed as memory-backed fioFILE streams, and pumps
// connect/disconnect/dispatch each frame via Update(). con::CONSOLE_SYS publicly inherits
// from this. DB-verified layout (types_members con::MSG_PROTOCOL) -- size 64.
// NOTE: this subsystem's prefix is "con", but the directory is "con_" because "con" is a
// Windows-reserved device name that git/Win32 cannot index.

namespace con {

struct MSG_PROTOCOL_vtbl; // boundary -- con subsystem vtable

typedef struct MSG_PROTOCOL {
    MSG_PROTOCOL_vtbl *__vftable;   // 0x00
    MSG_PROCESSOR *pComm;        // 0x04
    osLOCK           lock;        // 0x08
    int               isConnected; // 0x3C

    // --- vtable (declaration order == slot order; DB-verified con::MSG_PROTOCOL_vtbl;
    // virtual dispatch is through the explicit __vftable member above) ---
    // OnMsg is called with the request's page/ctrl/msg text and the request stream; `resp`
    // is non-null only for id==2 (response-expected) requests. A nonzero return selects
    // SendMsg id 3 (handled) vs id 4 (failed) for the reply. Subclass hook -- boundary.
    int  OnMsg(const dsTSTRING<char> *page, const dsTSTRING<char> *ctrl,
                        const dsTSTRING<char> *msg, fioFILE *req, fioFILE *resp); // slot 0 boundary (virtual)
    void OnConnected();     // slot 1 boundary (virtual) -- subclass hook, fired on pComm->isConnected 0->1
    void OnDisconnected();  // slot 2 boundary (virtual) -- subclass hook, fired on pComm->isConnected 1->0
    ~MSG_PROTOCOL();         // slot 3 boundary (virtual)

    // --- reversed (con::MSG_PROTOCOL batch) ---
    // Cancel/terminate a pending outbound message stream previously returned by StartMsg or
    // SendImmMsg: finds it in the shared message-buffer pool and calls Term() on it. 0x82AC85F8.
    void CancelMsg(fioFILE &f);

    // Claim (or grow) a pooled memory stream from the shared message-buffer pool, write the
    // page/ctrl/msg header strings into it, and return it ready for the caller to append a
    // payload and hand to SendMsg. 0x82AC8D88.
    fioFILE &StartMsg(const char *page, const char *ctrl, const char *msg);

    // Send a previously-started message stream (from StartMsg) to pComm as an id==1
    // (fire-and-forget) message, then release the pooled slot. 0x82AC8408.
    void SendMsg(fioFILE &f);

    // Send a previously-started message stream as an id==2 request and block for the
    // matching response via pComm->SendImmMsg, returning a pooled stream over the response
    // bytes (or null on failure/disconnect). 0x82AC8698.
    fioFILE *SendImmMsg(fioFILE &f);

    // Per-frame pump: drives the OnConnected/OnDisconnected transition hooks off
    // pComm->isConnected, then dispatches every queued inbound message to OnMsg (replying
    // via pComm->SendMsg for id==2 requests). Called once per frame by
    // con::CONSOLE_SYS::Update (@ 0x827EADD0). 0x82AC78D8.
    void Update();
} MSG_PROTOCOL;

// Shared pooled memory-stream buffer backing StartMsg/SendMsg/SendImmMsg/CancelMsg. In the
// original single-TU source (conComm.cpp) this was a private file-static (PDB confirms
// internal linkage; observed symbol name "_msgFile", real name "msgFile"); declared extern
// here since this reconstruction splits that TU across sibling .cpp files. Defined once, in
// MSG_PROTOCOL__CancelMsg.cpp.
extern dsVECTOR<fioFILE_MEM *, 8> msgFile;

// Paired reentrancy counter bound to SendImmMsg's con::SINGLE_ENTRY_CHECKER guard (also a
// private file-static in the original TU). Defined once, in MSG_PROTOCOL__CancelMsg.cpp.
extern int nEntry_0;

} // namespace con
