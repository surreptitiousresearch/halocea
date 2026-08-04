/* rasterizer_screen_flash @0x836A6118 — tail-call thunk forwarding to the underscore implementation. */

extern void _rasterizer_screen_flash(void);

void rasterizer_screen_flash(void)
{
    _rasterizer_screen_flash();
}
