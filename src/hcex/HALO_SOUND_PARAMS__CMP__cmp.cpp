#include "../headers/hcex/HALO_SOUND_PARAMS.h"

// @0x836B4DD0 — ?cmp@CMP@HALO_SOUND_PARAMS@@SAHABU2@0@Z
// Stateless ascending-by-tag comparator used by dsVECTOR<HALO_SOUND_PARAMS,8>::FindSorted / ds::Sort
// over haloSoundSystem->params / ->paramsWildcard. A plain ordinal strcmp over the interned tag
// names (a->tag vs b->tag), returning a-b of the first differing byte.
//
// DEVIATION: the decompiler renders both parameters as `const HALO_SOUND_PARAMS *` (its prototype
// string always shows references as pointers); the mangle decodes both as `ABU2@@` =
// reference-to-const, so the true signature takes `const HALO_SOUND_PARAMS &`.
int HALO_SOUND_PARAMS::CMP::cmp(const HALO_SOUND_PARAMS &a, const HALO_SOUND_PARAMS &b)
{
    const char *bs = b.tag.CStr();
    const char *as = a.tag.CStr();
    int result;
    do {
        result = (unsigned char)*as - (unsigned char)*bs;
        if (!*as)
            break;
        ++as;
        ++bs;
    } while (!result);
    return result;
}
