#include <stdint.h>
extern int _rasterizer_is_fullscreen(void);

int rasterizer_is_fullscreen(void)
{
    return _rasterizer_is_fullscreen();
}
