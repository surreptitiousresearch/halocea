#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::cbQOT_IsEmpty(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83251BC0
// ?cbQOT_IsEmpty@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "QOT_IsEmpty" getter callback: forward (argc, argv) to the virtual implQOT_IsEmpty and store
// the bool result into `retVal`.
void aiBRAIN::cbQOT_IsEmpty(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    bool empty = implQOT_IsEmpty(argc, argv);

    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<bool>(empty);
    retVal.StoreValue(boxed);
}
