#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::cbQOT_GetFront(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83249150
// ?cbQOT_GetFront@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL getter callback: store the brain's "quote-of-the-front" parameter list (implQOT_GetFront, a
// REF_TYPE<dsVECTOR_PARAM_LIST> handle) into `retVal`. `caller` is released on return.
void aiBRAIN::cbQOT_GetFront(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(implQOT_GetFront());
    // the caller ref releases itself at scope end.
}
