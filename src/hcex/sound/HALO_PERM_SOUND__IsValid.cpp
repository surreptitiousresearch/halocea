/* ?IsValid@HALO_PERM_SOUND@@QBA_NXZ @0x836B5010 */
// ?IsValid@HALO_PERM_SOUND@@QBA_NXZ -- 0x836B5020
#include "../../headers/hcex/HALO_PERM_SOUND.h"

bool HALO_PERM_SOUND::IsValid() const
{
    return fsbIndex != -1;
}
