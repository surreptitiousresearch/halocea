/* ?GetVolume@HALO_PERM_SOUND@@QBAMXZ @0x836B5028 */
// ?GetVolume@HALO_PERM_SOUND@@QBAMXZ -- 0x836B5028
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

float HALO_PERM_SOUND::GetVolume() const
{
    return params ? params->volume : 1.0f;
}
