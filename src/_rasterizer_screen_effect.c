/* _rasterizer_screen_effect @0x83686FB8 */
/* _rasterizer_screen_effect 0x83686FB8 — argument-forwarding tail-call thunk to the pixel-shader
 * screen-effect implementation _rasterizer_screen_effect_ps. */

#include "headers/rasterizer_screen_effect_parameters.h"

extern void _rasterizer_screen_effect_ps(const rasterizer_screen_effect_parameters *parameters);

void _rasterizer_screen_effect(const rasterizer_screen_effect_parameters *parameters)
{
    _rasterizer_screen_effect_ps(parameters);
}
