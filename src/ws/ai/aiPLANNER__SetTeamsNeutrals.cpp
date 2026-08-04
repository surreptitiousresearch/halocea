#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::SetTeamsNeutrals(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x83253988
// ?SetTeamsNeutrals@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z
//
// Run the descriptor's "SetTeamsNeutrals" SSL function with the two named teams, marshalled into a
// contiguous dsDATA[2]; returns its bool result.
bool aiPLANNER::SetTeamsNeutrals(const dsTSTRING<char> &teamIDA, const dsTSTRING<char> &teamIDB)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(teamIDA);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(teamIDB);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_SetTeamsNeutrals, 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
