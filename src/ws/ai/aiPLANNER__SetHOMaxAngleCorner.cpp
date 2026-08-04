#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiPLANNER::SetHOMaxAngleCorner(float) @ 0x83253ED8
// ?SetHOMaxAngleCorner@aiPLANNER@@QAAXM@Z
//
// Run the descriptor's "SetHOMaxAngleCorner" SSL function with a single float argument (no return
// value used).
void aiPLANNER::SetHOMaxAngleCorner(float angle)
{
    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<float>(angle);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    aiPLANNER_IA(this).sslObject.CallFunc(
        aiPLANNER_Desc(this)->sslFuncId_SetHOMaxAngleCorner, 1, &argData, nullptr, sslOBJ_REF());
    // argData destructor releases its stored value at scope end.
}
