#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbSetContourShootingDist(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324B238
// ?cbSetContourShootingDist@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetContourShootingDist" setter callback: read four floats out of argv[0..3] and forward them
// to the body via implSetContourShootingDist. `caller` is passed by value and released on return.
void aiBRAIN::cbSetContourShootingDist(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float d3 = 0.0f, d2 = 0.0f, d1 = 0.0f, d0 = 0.0f;
    argv[3].GetValue<float>(d3, DSD_CONV_RETRIEVE);
    argv[2].GetValue<float>(d2, DSD_CONV_RETRIEVE);
    argv[1].GetValue<float>(d1, DSD_CONV_RETRIEVE);
    argv[0].GetValue<float>(d0, DSD_CONV_RETRIEVE);
    implSetContourShootingDist(d0, d1, d2, d3);
    // the caller ref releases itself at scope end.
}
