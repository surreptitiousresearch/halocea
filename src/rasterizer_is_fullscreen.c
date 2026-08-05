#include <stdint.h>
extern int _rasterizer_is_fullscreen(void);

uint8_t rasterizer_is_fullscreen(void)
{
    return _rasterizer_is_fullscreen();
}
