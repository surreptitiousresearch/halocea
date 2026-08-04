#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::AddWeights @ 0x832655A0  ?AddWeights@aiDOM_SPAWN@@AAAXAAUm3dV@@M@Z
// Add an inverse-square proximity bias toward `center` to every spawn point's weight. The vertical
// component is doubled so height differences count double against a point.
void aiDOM_SPAWN::AddWeights(m3dV &center, float scale)
{
    for (int i = 0; i < this->pointsList.nElem; ++i)
    {
        dipPOINT &pt = this->pointsList[i];
        float dx = center.x - pt.pos.x;
        float dy = (center.y - pt.pos.y) * 2.0f;
        float dz = center.z - pt.pos.z;
        pt.weight = scale / ((dx * dx) + ((dz * dz) + (dy * dy)) + 0.1f) + pt.weight;
    }
}
