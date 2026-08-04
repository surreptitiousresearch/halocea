#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h"

// strmRESOURCE_TEXTURE::ResetMaxMipCounter @ 0x82BF6B20
// Every 5th call (counter is a multiple of 5), fold this period's per-mip view statistics into
// accDataVector.lastData.
void strmRESOURCE_TEXTURE::ResetMaxMipCounter()
{
    if (counter == 5 * (counter / 5))
        accDataVector.Merge();
}
