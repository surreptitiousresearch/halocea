// aiNAV_WP_WRAPPER_TPL::Get @ 0x8321ABB0
#include "../../headers/ws/ai/aiNAV_WP_WRAPPER_TPL.h"
#include "../../headers/ws/ai/aiNAV_WP_DATA.h"

// The template wrapper's Get is a default-fill stub: it constructs a fresh (empty) descriptor
// and assigns it into `data`, returning 0. (The binary inlines the aiNAV_WP_DATA ctor/dtor and
// the copy-assign; the decompiler recovered no index-dependent fill path.)
int aiNAV_WP_WRAPPER_TPL::Get(int /*idx*/, aiNAV_WP_DATA &data) const
{
    aiNAV_WP_DATA empty;
    data = empty;
    return 0;
}
