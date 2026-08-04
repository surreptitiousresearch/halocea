#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"
#include "../../headers/ws/ds/WILDCARD.h"

// HALO_SOUND_LIST::InitSetupTagParams() @ 0x836C0E98
// After `tags` is populated (by ParseBin/ParseLst), resolve each tag's HALO_SOUND_PARAMS:
// compile haloSoundSystem->paramsWildcard's tag-name patterns into a matching ds::WILDCARD
// list, then for each of our tags try an exact-name match against haloSoundSystem->params
// first, falling back to the first matching wildcard pattern (in paramsWildcard order).
void HALO_SOUND_LIST::InitSetupTagParams()
{
    dsVECTOR<ds::WILDCARD, 8> wildcards;
    wildcards.Resize(haloSoundSystem->paramsWildcard.nElem);

    for (int i = 0; i < wildcards.nElem; ++i)
    {
        const char *tagStr = haloSoundSystem->paramsWildcard[i].tag.CStr();
        dsTSTRING<char> tagText;
        tagText.pBuffer = nullptr;
        tagText.UnsafeInit(tagStr, -1, 0);
        wildcards[i].Init(&tagText, false);
    }

    for (int i = 0; i < this->tags.nElem; ++i)
    {
        HALO_SOUND_TAG &tag = this->tags[i];
        const char *tagName = tag.name.CStr();

        HALO_SOUND_PARAMS::CMP exactCmp = {};
        int exactIndex = haloSoundSystem->params.FindSorted<HALO_SOUND_PARAMS::CMP, const char *>(tagName, exactCmp);
        if (exactIndex != -1)
        {
            tag.params = &haloSoundSystem->params[exactIndex];
            continue;
        }

        dsTSTRING<char> tagNameText;
        tagNameText.pBuffer = nullptr;
        tagNameText.UnsafeInit(tagName, -1, 0);

        for (int w = 0; w < wildcards.nElem; ++w)
        {
            if (wildcards[w].Match(&tagNameText))
            {
                tag.params = &haloSoundSystem->paramsWildcard[w];
                break;
            }
        }
    }
}
