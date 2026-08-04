#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbHaveGroup(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83250290
// ?cbHaveGroup@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "HaveGroup" callback: forward (argc, argv) to implHaveGroup and store its bool result into
// `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbHaveGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool result = implHaveGroup(argc, argv);
    retVal.StoreValue<bool>(result);
    // the caller ref releases itself at scope end.
}
