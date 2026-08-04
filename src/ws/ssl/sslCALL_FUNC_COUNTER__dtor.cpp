#include "sslCALL_FUNC_COUNTER.h"
#include "sslSYSTEM.h"
#include "../con_/MSG_PROTOCOL.h"

// --- other-subsystem boundary (declared, not reversed here) ---
extern con::MSG_PROTOCOL *apConsole; // ap: active debug console (may be null; same convention as
                                      // dbgVAR_MANAGER__SendVarUpdate.cpp's `apConsole`)

// sslCALL_FUNC_COUNTER::~sslCALL_FUNC_COUNTER() @ 0x83087BA8
// Unwind the debugger single-step trap armed by the constructor: if we are back at the depth that
// armed it (dbgMode == 3 and the nesting counter has unwound to the latched Skip depth), drop back
// to paused (dbgMode 2); an unconditional latch state (4, set elsewhere while stepping) always
// drops back to 2. On the outermost unwind (nesting counter reaches 0), leave the system paused
// with no active debug function and notify the connected console debugger UI that tracing stopped.
sslCALL_FUNC_COUNTER::~sslCALL_FUNC_COUNTER()
{
    if (this->Ssl->dbgMode == 3 && sslCALL_FUNC_COUNTER::Count == sslCALL_FUNC_COUNTER::Skip)
        this->Ssl->dbgMode = 2;
    if (this->Ssl->dbgMode == 4)
        this->Ssl->dbgMode = 2;

    if (!--sslCALL_FUNC_COUNTER::Count)
    {
        sslSYSTEM *ssl = this->Ssl;
        if (ssl->dbgMode == 2)
        {
            ssl->dbgMode = 0;
            ssl->dbgFunc = nullptr;
            if (apConsole && apConsole->isConnected)
            {
                fioFILE &msg = apConsole->StartMsg("Ssl", "SSL_Dlg", "StopTrace");
                apConsole->SendMsg(msg);
            }
        }
    }
}
