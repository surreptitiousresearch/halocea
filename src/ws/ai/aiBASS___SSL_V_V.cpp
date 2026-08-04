// aiBASS::_SSL_V_V @ 0x83241560
// ?_SSL_V_V@aiBASS@@IAAXABVdsSTRID@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ssl/sslERROR.h"

// Invoke the scripted-sound object's `funcName` void()->void handler (no args, no return value).
void aiBASS::_SSL_V_V(const dsSTRID &funcName)
{
    sslObj.CallFunc(funcName, 0, nullptr, nullptr, sslOBJ_REF());
}
