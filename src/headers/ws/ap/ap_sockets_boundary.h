#pragma once
// ws-engine ap networking boundary declarations for ProcessSocketsNotifications.
// ap::BASE_SOCKET / ap::SOCKET_LISTENER are boundary types (only the virtual deleting-destructor
// and the static poll entry are used here); the module maintains a batch-delete list of sockets
// scheduled for destruction under a lock.

#include "../ds/LIST.h"
#include "../os/osLOCK.h"

namespace ap {

// Base network socket. boundary — only its virtual (deleting) destructor slot is invoked.
struct BASE_SOCKET {
    struct VTBL {
        void (*dtr)(BASE_SOCKET *self, int deletingFlag); // slot 0 — virtual ~BASE_SOCKET(int)
    };
    VTBL *__vftable; // 0x00
};

// Socket multiplexer. boundary — DoSelectNonBlock polls all listeners without blocking.
struct SOCKET_LISTENER {
    static void DoSelectNonBlock(); // ?DoSelectNonBlock@SOCKET_LISTENER@ap@@...
};

} // namespace ap

// Module globals (file-scope in the socket subsystem):
//   gs_delayedList     — sockets deferred for destruction this tick.
//   gs_delayedListLock — guards gs_delayedList.
//   gs_batchDelete     — set while draining so socket dtors route to the batch instead of
//                        recursing back into the list.
// DEVIATION: gs_batchDelete (0x842C15D0) is a 1-byte bool, not an int — the DB applied type is
// `bool gs_batchDelete;` and both accessors are byte ops at the symbol base (stb @0x82792F6C /
// 0x82792FD0 here, lbz @0x82793034 in ~BASE_SOCKET). On this big-endian target a 4-byte store of
// 1 would leave byte 0 zero, so the destructor's lbz would never observe the flag as set.
extern "C" ds::LIST<ap::BASE_SOCKET *> gs_delayedList;
extern "C" osLOCK                      gs_delayedListLock;
extern "C" bool                        gs_batchDelete;
