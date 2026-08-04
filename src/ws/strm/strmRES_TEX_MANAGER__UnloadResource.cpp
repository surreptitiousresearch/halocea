#include "../../headers/ws/strm/strmRES_TEX_MANAGER.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// strmRES_TEX_MANAGER::UnloadResource @ 0x82670FF8
// Request dropping `strmResource` down to `mip`. In mini-zoom mode with the resource already
// close enough (lastData.dist2.x < 5), skip the actual unload — just refresh the on-screen stats
// (curData[0].dist2.x) and mark the texture used-in-render. Otherwise: fail immediately while a
// load is already in flight (RES_STATE_LOADING); cancel any outstanding query first if one is
// pending (RES_STATE_IN_QUERY); then record removingMip and enqueue the resource on `unload`.
bool strmRES_TEX_MANAGER::UnloadResource(strmRESOURCE_TEXTURE *strmResource, int mip)
{
    if (miniZoomMode && strmResource->accDataVector.lastData.dist2.x < 5.0f) {
        strmResource->ownerTex->MarkAsUsedInRender();
        strmResource->accDataVector.curData.list[0].dist2.x = strmResource->accDataVector.lastData.dist2.x;
        return true;
    }

    if (strmResource->state == strmRESOURCE::RES_STATE_LOADING)
        return false;

    if (strmResource->state == strmRESOURCE::RES_STATE_IN_QUERY)
        strmResource->RemoveQuery();

    strmResource->removingMip = mip;
    unload.PushBack(strmResource);
    return true;
}
