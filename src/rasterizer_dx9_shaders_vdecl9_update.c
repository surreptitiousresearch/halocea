/* rasterizer_dx9_shaders_vdecl9_update @ 0x83724020
   Rebuilds the DX9 vertex declarations: dispose then re-initialize. */

#include <stdint.h>

extern void rasterizer_dx9_shaders_vdecl9_dispose(void);
extern unsigned int rasterizer_dx9_shaders_vdecl9_initialize(void);

uint8_t rasterizer_dx9_shaders_vdecl9_update(void)
{
    rasterizer_dx9_shaders_vdecl9_dispose();
    /* caller 836A2E14 byte-normalizes the threaded status (clrlwi r3,24) —
       declared return is uint8_t; truncation cast models that normalization */
    return (uint8_t)rasterizer_dx9_shaders_vdecl9_initialize();
}
