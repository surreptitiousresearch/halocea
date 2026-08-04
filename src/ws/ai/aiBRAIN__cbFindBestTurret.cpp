#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbFindBestTurret @ 0x8324F168
// ?cbFindBestTurret@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: read a float range from argv[0] (and a bool flag from argv[1], which the callback
// reads but does not forward — implFindBestTurret takes only the range), forward the range to
// implFindBestTurret, and store the returned turret-name string into `retVal`. `caller` releases on
// return.
void aiBRAIN::cbFindBestTurret(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool flag = false;
    argv[1].GetValue<bool>(flag, DSD_CONV_RETRIEVE); // isCheckAngles

    float range = 0.0f;
    argv[0].GetValue<float>(range, DSD_CONV_RETRIEVE);

    // implFindBestTurret is (float radius, bool isCheckAngles) per DB mangling — forward both.
    dsTSTRING<char> best = implFindBestTurret(range, flag);
    retVal.StoreValue<dsTSTRING<char> >(best);
    // best destructor releases the string buffer at scope end.
}
