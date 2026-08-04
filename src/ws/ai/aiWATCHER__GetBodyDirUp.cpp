// aiWATCHER::GetBodyDirUp @ 0x8328D808
// ?GetBodyDirUp@aiWATCHER@@UBA?AUm3dV@@XZ
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3dV.h"

// Virtual const getter: return the watched body's world-space up vector — the third (up) column of
// the body coordinate system (GetBodyCS).
m3dV *aiWATCHER::GetBodyDirUp(m3dV *result)
{
    m3dV pos;
    m3dV dir;
    GetBodyCS(&pos, &dir, result);
    return result;
}
