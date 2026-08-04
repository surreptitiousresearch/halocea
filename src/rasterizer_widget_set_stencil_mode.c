#include <stdint.h>
/* rasterizer_widget_set_stencil_mode @0x836A63C8 — public entry thunk. A single unconditional branch
 * (tail-call) to _rasterizer_widget_set_stencil_mode; forwards its argument unchanged.
 * Verified against disasm: `b _rasterizer_widget_set_stencil_mode`. */

extern void _rasterizer_widget_set_stencil_mode(int16_t stencil_mode);

void rasterizer_widget_set_stencil_mode(int16_t stencil_mode)
{
    _rasterizer_widget_set_stencil_mode(stencil_mode);
}
