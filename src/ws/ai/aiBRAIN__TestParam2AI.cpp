#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::TestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>) @ 0x8325B6F8
// ?TestParam2AI@aiBRAIN@@QAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z
//
// Run the descriptor's "TestParam2AI" SSL function with the param-list reference `params` as its
// single argument (no return value).
void aiBRAIN::TestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params)
{
    dsDATA paramData;
    paramData.type = nullptr;
    paramData.StoreValue<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> >(params);
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(paramData);

    // Void SSL op: no return value marshalled (retVal = null). RAII: the returned sslERROR temporary
    // and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnTestParam2AI(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg + paramData destructors self-release at scope end.
}
