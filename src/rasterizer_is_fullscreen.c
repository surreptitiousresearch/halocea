extern int _rasterizer_is_fullscreen(void);

unsigned __int8 rasterizer_is_fullscreen(void)
{
    return _rasterizer_is_fullscreen();
}
