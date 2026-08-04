#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::tIsFlag(const dsTSTRING<char>&) @ 0x832557D8
// ?tIsFlag@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's task-system "IsFlag" SSL function with `flagName` as its single argument;
// returns the script's bool result.
bool aiPLANNER::tIsFlag(const dsTSTRING<char> &flagName)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(flagName);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_tIsFlag, 1, &arg, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
