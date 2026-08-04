// aiBASS::_SSL_V_F @ 0x832419A0
// ?_SSL_V_F@aiBASS@@IAAXABVdsSTRID@@M@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslERROR.h"

// Invoke the scripted-sound object's `funcName` void(float) handler with a single float argument.
void aiBASS::_SSL_V_F(const dsSTRID &funcName, float arg1)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<float>(arg1);
    sslObj.CallFunc(funcName, 1, &arg, nullptr, sslOBJ_REF());
}
