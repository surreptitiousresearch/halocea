// aiBASS::InitLevel @ 0x83241928
// ?InitLevel@aiBASS@@UAAXXZ
#include "../../headers/ws/ai/aiBASS.h"

// Per-level init: fire the "OnInitLevel" script event on the scripted-sound object.
void aiBASS::InitLevel()
{
    static dsSTRID sslFN_OnInitLevel("OnInitLevel", false);
    _SSL_V_V(sslFN_OnInitLevel);
}
