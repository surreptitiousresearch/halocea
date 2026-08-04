#pragma once
// SESSION_BASE — ws-engine multiplayer session base (gs_session.h/.cpp). BOUNDARY receiver: only
// m_pRelay (@0x0C) is modeled at its DB-verified offset; the rest is an opaque tail sized to the
// DB total (576).
namespace net { struct RELAY; }
namespace mp { struct MESSAGE_DISPATCHER; }

struct SESSION_BASE {
    char        _pad0[12];   // 0x00 mp::LISTENER base + m_LocalParams + m_iSessionID
    net::RELAY *m_pRelay;    // 0x0C
    char        _tail[560];  // 0x10 .. 0x240 (total size 576)

    net::RELAY *GetRelay(); // 0x823BB0B8
    mp::MESSAGE_DISPATCHER *GetMessageDispatcher(); // boundary — this session's relay dispatcher
};
