#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbGetPathRemain @ 0x8324E2F0
// ?cbGetPathRemain@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: forward (argc, argv) to implGetPathRemain and store the resulting float into `retVal`.
// `caller` releases on return.
void aiBRAIN::cbGetPathRemain(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    // impl takes no args (DB ?implGetPathRemain@aiBRAIN@@UAAMXZ); argc/argv unused here
    float remaining = implGetPathRemain();
    retVal.StoreValue<float>(remaining);
}
