/* ?InitSetupTagParams@HALO_SOUND_LIST@@AAAXXZ @0x836C0E80 */
#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836C0E80 — ?InitSetupTagParams@HALO_SOUND_LIST@@AAAXXZ
// After `tags` is populated (ParseBin/ParseLst), resolve each tag's HALO_SOUND_PARAMS: an exact
// name match against haloSoundSystem->params (binary search) wins; otherwise the LAST matching
// ds::WILDCARD pattern from haloSoundSystem->paramsWildcard is applied (patterns compiled once up
// front, one per paramsWildcard entry).
void HALO_SOUND_LIST::InitSetupTagParams()
{
    dsVECTOR<ds::WILDCARD, 8> wildcards;
    wildcards.Resize(haloSoundSystem->paramsWildcard.nElem);
    for (int i = 0; i < wildcards.nElem; ++i) {
        dsTSTRING<char> pattern(haloSoundSystem->paramsWildcard[i].tag.CStr());
        wildcards[i].Init(&pattern, false);
    }

    for (int t = 0; t < this->tags.nElem; ++t) {
        HALO_SOUND_TAG *tag = &this->tags[t];

        HALO_SOUND_PARAMS::CMP cmp;
        const char *name = tag->name.CStr();
        int idx = haloSoundSystem->params.FindSorted<HALO_SOUND_PARAMS::CMP, const char *>(name, cmp);
        if (idx != -1) {
            tag->params = &haloSoundSystem->params[idx];
            continue;
        }

        // No exact match: apply every matching wildcard in order (last match wins).
        for (int w = 0; w < wildcards.nElem; ++w) {
            dsTSTRING<char> tagName(tag->name.CStr());
            if (wildcards[w].Match(&tagName))
                tag->params = &haloSoundSystem->paramsWildcard[w];
        }
    }
}
