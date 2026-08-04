#pragma once
// net::RELAY — the ws-engine networking relay (net_relay.h/.cpp). BOUNDARY receiver: only the
// fields touched by the methods re-sourced in this batch (idxLocalClient, clientData) are modeled
// with their DB-verified offsets; the remainder is an opaque tail sized to the DB total (372).
// CLIENT_REC likewise models only clientId (@8) inside a size-correct (284) blob so that
// dsVECTOR<CLIENT_REC,8>::operator[] indexing has the right element stride.
#include "CLIENT_ID.h"
#include "../ds/dsVECTOR.h"

namespace net {

struct RELAY {
    struct CLIENT_REC {
        char       _pad0[8];    // 0x00 state/generation
        CLIENT_ID  clientId;    // 0x08
        char       _tail[272];  // 0x0C .. 0x11C (total size 284)
    };

    char                    _pad0[20];       // 0x00
    int                     idxLocalClient;  // 0x14 index of this host's own client (-1 = none)
    char                    _pad24[24];      // 0x18
    dsVECTOR<CLIENT_REC, 8> clientData;      // 0x30 per-client records
    char                    _tail[304];      // 0x44 .. 0x174 (total size 372)

    CLIENT_ID GetLocalClientId() const; // 0x823C04E0
};

} // namespace net
