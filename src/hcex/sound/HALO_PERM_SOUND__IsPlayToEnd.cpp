/* ?IsPlayToEnd@HALO_PERM_SOUND@@QBA_NXZ @0x836B6880 */
// ?IsPlayToEnd@HALO_PERM_SOUND@@QBA_NXZ -- 0x836B6880
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

bool HALO_PERM_SOUND::IsPlayToEnd() const
{
    if (!params)
        return false;
    return (params->state.val & HALO_SOUND_PLAY_TO_END) != 0;
}
