#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbIsLive(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832495F8
// ?cbIsLive@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "IsLive" getter callback: store the brain's live/dead state (implIsLive) into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbIsLive(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool live = implIsLive();
    retVal.StoreValue<bool>(live);
    // the caller ref releases itself at scope end.
}
