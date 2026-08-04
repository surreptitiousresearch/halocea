#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbHasAnyAvailableStrike(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324B388
// ?cbHasAnyAvailableStrike@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "HasAnyAvailableStrike" getter callback: store implHasAnyAvailableStrike's bool result into
// `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbHasAnyAvailableStrike(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    // impl takes no args; the decompiler's forwarded (argc, argv) are dead callback registers.
    bool available = implHasAnyAvailableStrike();
    retVal.StoreValue<bool>(available);
    // the caller ref releases itself at scope end.
}
