// SESSION_BASE::GetRelay @0x823BB0B8 — accessor for the session's relay pointer.
#include "../../headers/ws/net/SESSION_BASE.h"

net::RELAY *SESSION_BASE::GetRelay()
{
    return m_pRelay;
}
