#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiSQUAD::cbGetGroup(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832A8338
// ?cbGetGroup@aiSQUAD@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
// SSL callback wrapper for GetGroup: box the native implGetGroup() ref into the return value.
void aiSQUAD::cbGetGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;

    sslOBJ_REF group = implGetGroup();

    dsDATA asData = group;   // sslOBJ_REF::operator dsDATA()
    dsDATA stored;
    stored.type = nullptr;
    stored.StoreValue(asData);

    retVal.StoreValue(stored);
    // asData/stored destructors + the by-value caller ref self-release at scope end.
}
