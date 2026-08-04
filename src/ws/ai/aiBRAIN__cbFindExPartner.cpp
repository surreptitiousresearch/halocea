#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiBRAIN::cbFindExPartner @ 0x8324D0B0
// ?cbFindExPartner@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: read a float search range from `argv`, forward it to implFindExPartner, and store
// the found partner actor's SSL object reference (empty when none) into `retVal`.
void aiBRAIN::cbFindExPartner(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    float range = 0.0f;
    argv->GetValue<float>(range, DSD_CONV_RETRIEVE);

    iaIACTOR *partner = implFindExPartner(range);

    sslOBJ_REF ref; // empty ref when no partner found
    if (partner)
        ref = partner->sslObject;

    dsDATA asData = ref; // sslOBJ_REF::operator dsDATA()
    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue(asData);
    retVal.StoreValue(tmp);
}
