// ?cmp@CMP@HALO_SOUND_PARAMS@@SAHABU2@PBD@Z -- @0x836B4E40
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

int HALO_SOUND_PARAMS::CMP::cmp(const HALO_SOUND_PARAMS &a, const char *name)
{
    const char *left = a.tag.CStr();

    int result;
    do
    {
        result = (unsigned char)*left - (unsigned char)*name;
        if (!*left)
            break;
        ++left;
        ++name;
    } while (!result);
    return result;
}
