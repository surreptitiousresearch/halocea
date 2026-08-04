/* rasterizer_environment_fog_screen_begin 0x836A6338 (thunk) — tail-call thunk to the underscore rasterizer implementation. */

#include <stdint.h>

extern void _rasterizer_environment_fog_screen_begin(int16_t pass);

void rasterizer_environment_fog_screen_begin(int16_t pass)
{
    _rasterizer_environment_fog_screen_begin(pass);
}
