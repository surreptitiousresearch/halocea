/* ?GetVolume2D@HALO_PERM_SOUND@@QBAMXZ @0x836B5048 */
// ?GetVolume2D@HALO_PERM_SOUND@@QBAMXZ -- 0x836B5048
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

float HALO_PERM_SOUND::GetVolume2D() const
{
    return params ? params->volume2D : 1.0f;
}
