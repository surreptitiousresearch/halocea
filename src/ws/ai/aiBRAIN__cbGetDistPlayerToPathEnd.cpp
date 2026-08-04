#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbGetDistPlayerToPathEnd @ 0x8324E448
// ?cbGetDistPlayerToPathEnd@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: forward (argc, argv) to implGetDistPlayerToPathEnd and store the resulting float into
// `retVal`. `caller` releases on return.
void aiBRAIN::cbGetDistPlayerToPathEnd(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    // impl takes no args (DB ?implGetDistPlayerToPathEnd@aiBRAIN@@UAAMXZ); argc/argv unused here
    float dist = implGetDistPlayerToPathEnd();
    retVal.StoreValue<float>(dist);
}
