#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine con: base class for console page callbacks. A named, versioned console page
// that the debug console can toggle and query.
// NOTE: this subsystem's prefix is "con", but the directory is "con_" because "con" is a
// Windows-reserved device name that git/Win32 cannot index.
// DB-verified layout (types_members con::CONSOLE_CALLBACK) -- size 16.

namespace con {

struct CONSOLE_CALLBACK_vtbl; // boundary -- con subsystem vtable

typedef struct CONSOLE_CALLBACK {
    CONSOLE_CALLBACK_vtbl *__vftable;         // 0x00
    dsTSTRING<char>        pageName;          // 0x04 console page name
    int                    isVisible;         // 0x08
    int                    pageVersionValid;  // 0x0C

    CONSOLE_CALLBACK(); // 0x827EA400

    // Non-const, void, no args -- boundary. Called on every registered callback by
    // con::CONSOLE_SYS::Update before message processing.
    void Update();               // boundary
    // Called on every registered callback by con::CONSOLE_SYS::Update after message
    // processing (con::CONSOLE_SYS::Update @ 0x827EADD0).
    void UpdateAfterMessages();  // boundary
} CONSOLE_CALLBACK;

} // namespace con
