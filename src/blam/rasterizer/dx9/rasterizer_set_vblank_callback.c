/* rasterizer_set_vblank_callback @0x836A5DC0 */
extern void _rasterizer_set_vblank_callback(void (*callback)(unsigned int));

void rasterizer_set_vblank_callback(void (*callback)(unsigned int))
{
    _rasterizer_set_vblank_callback(callback);
}
