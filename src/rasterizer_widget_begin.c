/* rasterizer_widget_begin 0x836A63B0 (thunk) — tail-call thunk to the underscore implementation. */
#include <stdint.h>

extern void _rasterizer_widget_begin(int16_t type, uint16_t flags);

void rasterizer_widget_begin(int16_t type, uint16_t flags)
{
    _rasterizer_widget_begin(type, flags);
}
