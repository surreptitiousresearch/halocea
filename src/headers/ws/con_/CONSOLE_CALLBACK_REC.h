#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine con: one registration record in con::CONSOLE_SYS::listCb -- a named
// callback page plus the callback object itself.
// DB-verified layout (types_members con::CONSOLE_CALLBACK_REC) -- size 8.

namespace con {

struct CONSOLE_CALLBACK; // ../con_/CONSOLE_CALLBACK.h (avoid circular include; used by pointer only)

typedef struct CONSOLE_CALLBACK_REC {
    dsTSTRING<char>    name; // 0x00 registration name
    CONSOLE_CALLBACK  *pCb;  // 0x04
} CONSOLE_CALLBACK_REC;

} // namespace con
