/* rasterizer_set_technique @ 0x8369CBE0
   Activates a technique on an effect. */

#include "headers/d3dx_effect_boundary.h"

long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique)
{
    return ID3DXEffect_SetTechnique(effect, technique);  /* C boundary wrapper for opaque COM object */
}
