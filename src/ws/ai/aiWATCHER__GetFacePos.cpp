// aiWATCHER::GetFacePos @ 0x8328D918
// ?GetFacePos@aiWATCHER@@UBA?AUm3dV@@XZ
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3dV.h"

extern const m3dV m3dVZero;                       // ?m3dVZero@@3Um3dV@@A

// Virtual const getter: the watched body's face/eye world position — the position out arg of
// GetFaceParams (the direction out arg is discarded here).
m3dV *aiWATCHER::GetFacePos(m3dV *result)
{
    m3dV dir;
    *result = m3dVZero;                 // defensive init (overwritten by GetFaceParams)
    dir = m3dVZero;
    GetFaceParams(result, &dir);
    return result;
}
