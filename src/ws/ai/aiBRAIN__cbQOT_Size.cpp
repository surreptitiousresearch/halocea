#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbQOT_Size(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83251A70
// ?cbQOT_Size@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "QOT_Size" getter callback: forward (argc, argv) to the virtual implQOT_Size and store the int
// queue size into `retVal`. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbQOT_Size(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    int size = implQOT_Size(argc, argv);

    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<int>(size);
    retVal.StoreValue(boxed);
    // the caller ref releases itself at scope end.
}
