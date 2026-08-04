#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine con: one pending console message — a message-type id plus a raw payload buffer.
// DB-verified layout (types_members con::CON_MSG) -- size 24.
// Known id values observed in MSG_PROCESSOR_BASE usage: 2 = outgoing/immediate request,
// 3 = response, 4 = terminator/disconnect marker (exact enum not recovered here).

namespace con {

typedef struct CON_MSG {
    int                             id;    // 0x00 message type
    dsVECTOR<unsigned char, 8>      data;  // 0x04 raw payload bytes
} CON_MSG;

} // namespace con
