#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbSetBHVOnEmptyStack(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324C748
// ?cbSetBHVOnEmptyStack@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetBHVOnEmptyStack" callback: forward (argc, argv) to implSetBHVOnEmptyStack and store its
// bool result into `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbSetBHVOnEmptyStack(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool result = implSetBHVOnEmptyStack(argc, argv);
    retVal.StoreValue<bool>(result);
    // the caller ref releases itself at scope end.
}
