#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbGetGroup @ 0x83248C68
// ?cbGetGroup@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the brain's coordinated-group SSL object reference (an empty ref when the
// brain is in no group) into `retVal`.
void aiBRAIN::cbGetGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    sslOBJ_REF ref = implGetGroup();

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
