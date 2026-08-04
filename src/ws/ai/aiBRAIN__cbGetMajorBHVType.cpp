#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetMajorBHVType(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324BF30
// ?cbGetMajorBHVType@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetMajorBHVType" getter callback: store the brain's major behaviour type name
// (implGetMajorBHVType) into `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetMajorBHVType(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implGetMajorBHVType());
    // the implGetMajorBHVType() temporary and the caller ref release themselves at scope end.
}
