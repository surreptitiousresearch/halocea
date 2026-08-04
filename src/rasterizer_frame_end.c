/* rasterizer_frame_end @0x836A5DA8 — thunk to the platform rasterizer frame-end implementation. */

extern void _rasterizer_frame_end(void);

void rasterizer_frame_end(void)
{
    _rasterizer_frame_end();
}
