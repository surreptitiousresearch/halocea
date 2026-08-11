// ?cmp@CMP@HALO_SOUND_TAG@@QBAHABU2@PBD@Z -- @0x836B4EE0
#include "../../headers/hcex/HALO_SOUND_TAG.h"

extern "C" int stricmp(const char *a, const char *b);

int HALO_SOUND_TAG::CMP::cmp(const HALO_SOUND_TAG &a, const char *name) const
{
    return stricmp(a.name.CStr(), name);
}
