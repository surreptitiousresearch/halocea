#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"

// aiWATCHER_PLAYER::GetBodyCS @ 0x8328DC58
// ?GetBodyCS@aiWATCHER_PLAYER@@UBAXAAUm3dV@@00@Z
// Virtual const: forward the player's cached body coordinate system.
void aiWATCHER_PLAYER::GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp) const
{
    pos = cachedBodyPos;
    dir = cachedBodyDir;
    dirUp = cachedBodyUp;
}
