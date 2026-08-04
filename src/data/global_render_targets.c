#include "../headers/rasterizer_render_target.h"
/* rasterizer_render_target[10]; bytes decoded big-endian:
 * per entry { width, height, _D3DFORMAT format, surface=NULL, texture=NULL } */
rasterizer_render_target global_render_targets[10] = {
    { 640, 480, (_D3DFORMAT)0x18280186, 0, 0 },
    { 640, 480, (_D3DFORMAT)0x18280186, 0, 0 },
    { 320, 240, (_D3DFORMAT)0x18280186, 0, 0 },
    { 128, 128, (_D3DFORMAT)0x28280186, 0, 0 },
    { 128, 128, (_D3DFORMAT)0x28280186, 0, 0 },
    {  64,  64, (_D3DFORMAT)0x28280186, 0, 0 },
    {  64,  64, (_D3DFORMAT)0x18280186, 0, 0 },
    {  64,  64, (_D3DFORMAT)0x28280186, 0, 0 },
    { 128, 128, (_D3DFORMAT)0x28280186, 0, 0 },
    { 640, 480, (_D3DFORMAT)0x2D200196, 0, 0 },
};
