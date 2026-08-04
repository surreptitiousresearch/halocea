#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetViewUpCurve(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832575E0
// ?cbSetViewUpCurve@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetViewUpCurve" setter callback: unbox the curve-name string out of argv[0] and forward it
// to the virtual implSetViewUpCurve. `caller` is passed by value and self-releases on return.
void aiBRAIN::cbSetViewUpCurve(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetViewUpCurve(name.val);
}
