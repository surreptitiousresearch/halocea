#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbGetClosestPoint(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832580C8
// ?cbGetClosestPoint@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetClosestPoint" callback: unbox (points, filter) out of argv and forward to the virtual
// implGetClosestPoint; store its returned name string into `retVal`. `caller` self-releases.
void aiBRAIN::cbGetClosestPoint(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > points(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > filter(argv[1], nullptr);

    dsTSTRING<char> name = implGetClosestPoint(points.val, filter.val);
    retVal.StoreValue<dsTSTRING<char> >(name);
}
