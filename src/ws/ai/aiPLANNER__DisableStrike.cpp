#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::DisableStrike(const dsTSTRING<char>&) @ 0x83254F98
// ?DisableStrike@aiPLANNER@@QAAXABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "DisableStrike" SSL function with `strikeName` as its single argument (no
// return value used).
void aiPLANNER::DisableStrike(const dsTSTRING<char> &strikeName)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(strikeName);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_DisableStrike, 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
