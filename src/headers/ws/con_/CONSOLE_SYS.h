#pragma once
#include "MSG_PROTOCOL.h"
#include "CONSOLE_CALLBACK_REC.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../fio/fioFILE_MEM.h"
// ws-engine con: the top-level console system -- owns the registered console page
// callbacks, the network message protocol (base class), and the connection/session
// state used by the in-game debug console.
// NOTE: directory is "con_" (not "con") because "con" is a Windows-reserved device name.
// DB-verified layout (types_members con::CONSOLE_SYS) -- size 236.

namespace con {

// Anonymous leading member @0, size 64, type con::MSG_PROTOCOL -> modeled as public
// inheritance (promotes members/vtable, matches layout).
struct CONSOLE_SYS : MSG_PROTOCOL {
    dsVECTOR<CONSOLE_CALLBACK_REC, 8> listCb;          // 0x40 registered console-page callbacks
    fioFILE_MEM                        immResponse;     // 0x54 immediate-mode response stream
    int                                 pause_mode;      // 0xD0
    int                                 frameNmb;        // 0xD4 last frame number passed to Update
    int                                 isServer;        // 0xD8
    int                                 m_haveErrorInConnect; // 0xDC
    int                                 m_isConnecting;  // 0xE0
    dsTSTRING<char>                     m_strAddress;    // 0xE4
    dsTSTRING<char>                     m_strWorkspace;  // 0xE8

    // Per-frame tick: `frame` == -1 keeps the previously recorded frameNmb. Runs every
    // registered callback's Update, pumps the base MSG_PROTOCOL, then runs every
    // registered callback's UpdateAfterMessages.
    void Update(int frame); // 0x827EADD0
};

} // namespace con
