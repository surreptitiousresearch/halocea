/* global_render_targets @ 0x84176828 (.data, 200 bytes)
 * DB applied_types: rasterizer_render_target global_render_targets[10];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 width                      = 0x00000280
 *        +0x04 height                     = 0x000001E0
 *        +0x08 format                     = 0x18280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 1] +0x00 width                      = 0x00000280
 *        +0x04 height                     = 0x000001E0
 *        +0x08 format                     = 0x18280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 2] +0x00 width                      = 0x00000140
 *        +0x04 height                     = 0x000000F0
 *        +0x08 format                     = 0x18280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 3] +0x00 width                      = 0x00000080
 *        +0x04 height                     = 0x00000080
 *        +0x08 format                     = 0x28280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 4] +0x00 width                      = 0x00000080
 *        +0x04 height                     = 0x00000080
 *        +0x08 format                     = 0x28280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 5] +0x00 width                      = 0x00000040
 *        +0x04 height                     = 0x00000040
 *        +0x08 format                     = 0x28280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 6] +0x00 width                      = 0x00000040
 *        +0x04 height                     = 0x00000040
 *        +0x08 format                     = 0x18280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   [ 7] +0x00 width                      = 0x00000040
 *        +0x04 height                     = 0x00000040
 *        +0x08 format                     = 0x28280186
 *        +0x0C surface                    = 0x00000000
 *        +0x10 texture                    = 0x00000000
 *   ... 2 further elements elided; full hex in .sweep/data_image.tsv
 */
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
