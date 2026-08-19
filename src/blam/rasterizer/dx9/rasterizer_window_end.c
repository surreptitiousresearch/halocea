/* rasterizer_window_end @0x836A5D98 — thunk to the platform rasterizer window-end implementation. */

extern void _rasterizer_window_end(void);

void rasterizer_window_end(void)
{
    _rasterizer_window_end();
}
