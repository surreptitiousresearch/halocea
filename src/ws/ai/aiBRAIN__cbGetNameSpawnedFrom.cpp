#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::cbGetNameSpawnedFrom(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324C7D8
// ?cbGetNameSpawnedFrom@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetNameSpawnedFrom" getter callback: store the name this brain was spawned from
// (implGetNameSpawnedFrom) into `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbGetNameSpawnedFrom(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<dsTSTRING<char> >(implGetNameSpawnedFrom());
    // the implGetNameSpawnedFrom() temporary and the caller ref release themselves at scope end.
}
