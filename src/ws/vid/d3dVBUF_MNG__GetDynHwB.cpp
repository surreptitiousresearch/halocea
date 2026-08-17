/* ?GetDynHwB@d3dVBUF_MNG@@UBAPAXW4vidBUF_CONTENT@@@Z @0x827CEC10 */
#include "../../headers/ws/vid/d3dVBUF_MNG.h"

// 0x827CEC10 -- ?GetDynHwB@d3dVBUF_MNG@@UBAPAXW4vidBUF_CONTENT@@@Z (virtual, const)
// Return the platform hardware-buffer header for the current dynamic vidBUFFER of the given
// content kind. The hardware buffer object is stored 0x68 bytes before the vidBUFFER record
// (the decompiler renders this as `&pBuf[-7].allocated`, i.e. -7*sizeof(vidBUFFER=16)+8 = -0x68);
// returns nullptr when no dynamic buffer is currently bound for `cont`.
void *d3dVBUF_MNG::GetDynHwB(vidBUF_CONTENT cont) const
{
    vidBUFFER *pBuf = this->curDynBuf[cont];
    if (!pBuf)
        return nullptr;
    return reinterpret_cast<void *>(reinterpret_cast<char *>(pBuf) - 0x68);
}
