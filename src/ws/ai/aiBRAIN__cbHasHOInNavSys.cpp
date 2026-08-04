#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbHasHOInNavSys @ 0x8324DD50
// ?cbHasHOInNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: forward (argc, argv) to implHasHOInNavSys and store the resulting bool into `retVal`.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbHasHOInNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool result = implHasHOInNavSys(argc, argv);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
    // tmp destructor releases its stored value at scope end.
}
