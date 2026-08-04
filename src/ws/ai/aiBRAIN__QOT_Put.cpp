#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::QOT_Put(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>) @ 0x832594C0
// ?QOT_Put@aiBRAIN@@QAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z
//
// Run the descriptor's "QOT_Put" SSL function ("quote-of-the-front" queue push) with the param-list
// reference `params` as its single argument (no return value).
void aiBRAIN::QOT_Put(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params)
{
    dsDATA paramData;
    paramData.type = nullptr;
    paramData.StoreValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(params);
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(paramData);

    // Void SSL op: no return value marshalled (retVal = null). RAII: the returned sslERROR temporary
    // and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnQOT_Put(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg + paramData destructors self-release at scope end.
}
