// aiWATCHER::GetBodyPos @ 0x8328D7B8
// ?GetBodyPos@aiWATCHER@@UBA?AUm3dV@@XZ
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3dV.h"

// Virtual const getter: return the watched body's world position — the position column of the
// body coordinate system (GetBodyCS). ABI-flattened form (m3dV out slot, returns it) matches the
// consumers (aiBRAIN::implCountMyNeutralsAroundEnemy, aiFILT_ENEMY::GetVisCheckParams).
m3dV *aiWATCHER::GetBodyPos(m3dV *result)
{
    m3dV dir;
    m3dV up;
    GetBodyCS(result, &dir, &up);
    return result;
}
