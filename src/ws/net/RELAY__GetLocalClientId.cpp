// net::RELAY::GetLocalClientId @0x823C04E0 — return the CLIENT_ID of this host's own client
// record. Strong-asserts (unless suppressed) that a local client is actually assigned.
#include "../../headers/ws/net/RELAY.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

namespace net {

CLIENT_ID RELAY::GetLocalClientId() const
{
    if (!IGNORE_STRONG_ASSERT && idxLocalClient < 0)
        STRONG_ASSERT_DUMMY().Crash(
            "idxLocalClient >= 0",
            "D:\\Projects\\code\\common\\incl.sys\\DRV\\net_relay.h",
            188,
            dsStrongAssertMessage);
    return clientData[idxLocalClient].clientId;
}

} // namespace net
