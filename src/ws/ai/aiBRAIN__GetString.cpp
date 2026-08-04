#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::GetString(const dsTSTRING<char>&) @ 0x83258CC8
// ?GetString@aiBRAIN@@QAA?AV?$dsTSTRING@D@@ABV2@@Z
//
// Run the descriptor's "GetString" SSL function with `key` as its single string argument; unbox the
// returned dsDATA back into a dsTSTRING<char> and return it by value.
dsTSTRING<char> aiBRAIN::GetString(const dsTSTRING<char> &key)
{
    dsDATA keyData;
    keyData.type = nullptr;
    keyData.StoreValue<dsTSTRING<char> >(key);
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue(keyData);

    dsDATA ret;
    ret.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetString(spDesc.pointee), 1, &arg, &ret, sslOBJ_REF());

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > result(ret, nullptr);
    return result.val;
    // result + ret + arg + keyData destructors self-release at scope end.
}
