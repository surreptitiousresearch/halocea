// aiBASS::ProcessFrame @ 0x83241CB8
// ?ProcessFrame@aiBASS@@UAAXM@Z
#include "../../headers/ws/ai/aiBASS.h"

// Per-frame tick: advance the exclamation stream, then fire the "OnFrame" script event.
// (Disasm-verified: `dt` is preserved in f1 across the ProcessStream call and reused for OnFrame.)
void aiBASS::ProcessFrame(float dt)
{
    ProcessStream(dt);
    static dsSTRID sslFN_OnFrame("OnFrame", false);
    _SSL_V_F(sslFN_OnFrame, dt);
}
