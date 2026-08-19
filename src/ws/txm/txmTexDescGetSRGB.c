#include "headers/ws/txm/txm_functions.h"

// @0x82C65C70 -- true when `desc` selects an sRGB-encoded usage (bit 0x1 or 0x10000 of
// usage.maps) and rendering.linearRGB hasn't overridden that to linear.
bool txmTexDescGetSRGB(const ENGINE_TEX_DESC &desc)
{
    if ((desc.usage.maps & 0x10003) == 0)
        return false;

    return !desc.rendering.linearRGB;
}
