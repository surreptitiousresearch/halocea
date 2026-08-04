#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::NavSysPenalizeByDomain(const dsTSTRING<char>&, const dsTSTRING<char>&, int) @ 0x832533A8
// SSL op: raise the traversal penalty for domain nameDom in nav-system nameNS.
// Marshals the arguments into a contiguous dsDATA[3] and runs the descriptor's "NavSysPenalizeByDomain" SSL
// function via sslObject.CallFunc, on behalf of an empty (script-root) caller.
bool aiPLANNER::NavSysPenalizeByDomain(const dsTSTRING<char> &nameNS, const dsTSTRING<char> &nameDom, int domain)
{
    dsDATA args[3];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(nameNS);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(nameDom);
    args[2].type = nullptr;
    args[2].StoreValue<int>(domain);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_SslObject(this)->CallFunc(aiPLANNER_DESC_SslFnNavSysPenalizeByDomain(aiPLANNER_SpDescPointee(this)), 3, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
