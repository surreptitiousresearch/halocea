// ?cmp@CMP@HALO_SOUND_PARAMS@@SAHABU2@0@Z -- 0x836B4DF0
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

int HALO_SOUND_PARAMS::CMP::cmp(const HALO_SOUND_PARAMS &a, const HALO_SOUND_PARAMS &b)
{
    const char *left = a.tag.CStr();
    const char *right = b.tag.CStr();

    int result;
    do
    {
        result = (unsigned char)*left - (unsigned char)*right;
        if (!*left)
            break;
        ++left;
        ++right;
    } while (!result);
    return result;
}
