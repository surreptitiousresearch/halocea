#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h"
#include "../../headers/ws/strm/strmSTREAMING_MANAGER.h"

// strmRESOURCE_TEXTURE::RemoveQuery @ 0x82BF5E58
// Cancel any outstanding streaming query for this resource, drop back to RES_STATE_IDLE, and
// clear loadingMip. Always succeeds.
int strmRESOURCE_TEXTURE::RemoveQuery()
{
    strmMng.RemoveQuery(this);
    SetState(RES_STATE_IDLE);
    loadingMip = 0;
    return 1;
}
