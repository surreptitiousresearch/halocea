#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORD_GROUP::cbGetType @ 0x8322F758
// ?cbGetType@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: return the group's type string as the callback's return value.
void aiCOORD_GROUP::cbGetType(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    // GetType() returns a pointer to the group's stored type name; the copy is ref-counted (the
    // manual buffer refCount inc/dec + dlFree in the binary is the inlined dsTSTRING copy-ctor/dtor).
    dsTSTRING<char> type = *GetType();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<dsTSTRING<char> >(type);
    retVal.StoreValue(tmp);
}
