/* rasterizer_screen_effect 0x836A6110 — thunk to the screen-effect implementation. */

#include "headers/rasterizer_screen_effect_parameters.h"

extern void _rasterizer_screen_effect(const rasterizer_screen_effect_parameters *parameters);

void rasterizer_screen_effect(const rasterizer_screen_effect_parameters *parameters)
{
    _rasterizer_screen_effect(parameters);
}
