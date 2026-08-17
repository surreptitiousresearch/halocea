/* ?DiscardDynAll@d3dVBUF_MNG@@UAAXXZ @0x827CD738 */
#include "../../headers/ws/vid/d3dVBUF_MNG.h"
#include "../../headers/ws/vid/vidBUFFER.h"

// 0x827CD738 -- ?DiscardDynAll@d3dVBUF_MNG@@UAAXXZ (virtual)
// Discard all dynamic geometry allocated this frame by rewinding the write cursor of both the
// dynamic vertex buffer and the dynamic index buffer back to the start.
void d3dVBUF_MNG::DiscardDynAll()
{
    this->GetDynBuf(VID_BC_VERT)->curOfs = 0;
    this->GetDynBuf(VID_BC_INDEX)->curOfs = 0;
}
