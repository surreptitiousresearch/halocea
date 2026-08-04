// aiNAV_WP_DATA::GetMatrix @ 0x8321A598
#include "../../headers/ws/ai/aiNAV_WP_DATA.h"
#include "../../headers/ws/m3d/m3dMATR.h"

bool aiNAV_WP_DATA::GetMatrix(m3dMATR &_mat) const
{
    if (haveMatrix)
        _mat = matrix; // inlined 64-byte field copy in the binary
    return haveMatrix;
}
