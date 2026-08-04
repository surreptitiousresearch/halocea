#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbGetDistToWP(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83257DC8
// ?cbGetDistToWP@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetDistToWP" callback: unbox the waypoint name out of argv[0] and forward to the virtual
// implGetDistToWP; store its float result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbGetDistToWP(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > wp(argv[0], nullptr);

    float dist = implGetDistToWP(wp.val);
    retVal.StoreValue<float>(dist);
}
