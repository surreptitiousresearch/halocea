/* ?SetDesc@txmTEXTURE@@IAAXABUENGINE_TEX_DESC@@@Z @0x826F6D78 */
#include "headers/ws/txm/txmTEXTURE.h"
#include "headers/ws/txm/txm_functions.h"

// 0x826F6D84
void txmTEXTURE::SetDesc(const ENGINE_TEX_DESC &_desc)
{
    this->desc = _desc;

    if (((this->state.state >> 6) & 1) == 0 && txmTexDescGetSRGB(_desc))
        this->state.state |= 0x10u;

    this->UpdateTextureSwizzle();

    if ((_desc.usage.maps & 0x18) != 0)
        this->state.state |= 0x4000u;

    if ((_desc.usage.maps & 0x10000) != 0)
        this->state.state |= 0x800u;

    if ((_desc.usage.maps & 0x6000) != 0)
    {
        this->desc.mapping.addressU = ETADDR_WRAP;
        this->desc.mapping.addressV = ETADDR_WRAP;
    }
}
