#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::cbTestParamFromAI(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832490B0
// ?cbTestParamFromAI@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL getter callback: store the brain's AI-test parameter list (implTestParamFromAI, a
// REF_TYPE<dsVECTOR_PARAM_LIST> handle) into `retVal`. `caller` is released on return.
void aiBRAIN::cbTestParamFromAI(int /*argc*/, dsDATA * /*argv*/, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    retVal.StoreValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(implTestParamFromAI());
    // the caller ref releases itself at scope end.
}
