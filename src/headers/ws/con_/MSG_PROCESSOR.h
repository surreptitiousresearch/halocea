#pragma once
#include "MSG_PROCESSOR_BASE.h"
// ws-engine con: con::MSG_PROCESSOR -- the concrete message-transport backend that
// con::MSG_PROTOCOL drives (network/local console pipe worker thread). Boundary class --
// external to this batch; con::MSG_PROCESSOR_BASE (the part MSG_PROTOCOL actually
// touches: SendMsg/HaveMsg/GetMsg/SendImmMsg/isConnected) is owned by a sibling task and
// already reversed in MSG_PROCESSOR_BASE.h -- reused here, not redefined.
// NOTE: this subsystem's prefix is "con", but the directory is "con_" because "con" is a
// Windows-reserved device name that git/Win32 cannot index.

namespace con {

struct osHANDLE_DUMMY; // boundary — opaque OS handle

// DB-verified layout (types_members con::MSG_PROCESSOR): base @0 (136B) + sigExit@136, hThread@140.
struct MSG_PROCESSOR : MSG_PROCESSOR_BASE {
    osHANDLE_DUMMY *sigExit;  // 0x88
    osHANDLE_DUMMY *hThread;  // 0x8C

    // MSG_PROCESSOR_vtbl (Init/Term/HwSendBuf/HwReceBuf/HwGetLastError/ThreadProc -- an
    // 8-slot vtable replacing the base's 2-slot MSG_PROCESSOR_BASE_vtbl) is not expanded here:
    // con::MSG_PROTOCOL only ever calls through the base's SendMsg/HaveMsg/GetMsg/SendImmMsg
    // and reads pComm->isConnected, never anything MSG_PROCESSOR itself adds.
};

} // namespace con
