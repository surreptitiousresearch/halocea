/* ?GetVSync@vidCONFIG@@QBAHXZ @0x82BE9778 */
#include "headers/ws/vid/vidCONFIG.h"

// 0x82BE9778
int vidCONFIG::GetVSync() const
{
    if (this->forceVSync == VSYNC_FORCE_OFF)
        return 0;
    if (this->forceVSync == VSYNC_FORCE_ON)
        return 1;

    return this->useVSync;
}
