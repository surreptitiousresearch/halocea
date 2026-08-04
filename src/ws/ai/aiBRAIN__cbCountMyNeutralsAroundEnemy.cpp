#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbCountMyNeutralsAroundEnemy(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83250C40
// ?cbCountMyNeutralsAroundEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "CountMyNeutralsAroundEnemy" getter callback: read a float radius out of `argv[0]`, forward it
// to implCountMyNeutralsAroundEnemy, and store the int result into `retVal`. `caller` is passed by
// value and its ref is released on return.
void aiBRAIN::cbCountMyNeutralsAroundEnemy(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    float radius = 0.0f;
    argv->GetValue<float>(radius, DSD_CONV_RETRIEVE);
    int count = implCountMyNeutralsAroundEnemy(radius);
    retVal.StoreValue<int>(count);
    // the caller ref releases itself at scope end.
}
