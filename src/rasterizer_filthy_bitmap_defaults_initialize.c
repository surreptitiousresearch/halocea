/* rasterizer_filthy_bitmap_defaults_initialize @0x8369... — create the rasterizer's fallback ("filthy", i.e.
 * placeholder) textures used whenever a real bitmap is missing: a 4x4 fully-white 2D texture, a 4x4 2D, a
 * 4x4x4 volume and a 4x4x6 cube texture, the latter three filled with an alternating two-colour pattern in
 * A4R4G4B4. The four textures are stored in rasterizer_globals as the default white / 2D / 3D / cube formats.
 * If any texture fails to create, the fills are skipped but the (possibly null) handles are still stored.
 *
 * DEVIATION: the original took an unused argument and the decompiler modelled a convoluted bit-twiddle for the
 * four non-null checks; reproduced as a plain four-way null test. The function neither reads its argument nor
 * returns a value, so it is written as void(void). */

#include <stdint.h>
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_globals.h"
#include "headers/blam_data_globals.h"


void rasterizer_filthy_bitmap_defaults_initialize(void)
{
    D3DTexture *white = (D3DTexture *)D3DDevice_CreateTexture(4, 4, 1, 1, 0, D3DFMT_A4R4G4B4, 1, D3DRTYPE_TEXTURE);
    D3DTexture *texture_2d = (D3DTexture *)D3DDevice_CreateTexture(4, 4, 1, 1, 0, D3DFMT_A4R4G4B4, 1, D3DRTYPE_TEXTURE);
    D3DVolumeTexture *texture_3d =
        (D3DVolumeTexture *)D3DDevice_CreateTexture(4, 4, 4, 1, 0, D3DFMT_A4R4G4B4, 1, D3DRTYPE_VOLUMETEXTURE);
    D3DCubeTexture *texture_cube =
        (D3DCubeTexture *)D3DDevice_CreateTexture(4, 4, 6, 1, 0, D3DFMT_A4R4G4B4, 1, D3DRTYPE_CUBETEXTURE);

    if (white && texture_2d && texture_3d && texture_cube)
    {
        const unsigned short pattern[2] = { 0x0F00, 0xF0F0 };
        _D3DLOCKED_RECT locked_rect;
        _D3DLOCKED_BOX locked_box;

        D3DTexture_LockRect(white, 0, &locked_rect, nullptr, 0);
        for (int texel = 0; texel < 16; ++texel)
            ((unsigned short *)locked_rect.pBits)[texel] = 0xFFFF;
        D3DTexture_UnlockRect(white, 0);

        D3DTexture_LockRect(texture_2d, 0, &locked_rect, nullptr, 0);
        for (int texel = 0; texel < 16; ++texel)
            ((unsigned short *)locked_rect.pBits)[texel] = pattern[texel & 1];
        D3DTexture_UnlockRect(texture_2d, 0);

        D3DVolumeTexture_LockBox(texture_3d, 0, &locked_box, nullptr, 0);
        for (int texel = 0; texel < 64; ++texel)
            ((unsigned short *)locked_box.pBits)[texel] = pattern[texel & 1];
        D3DVolumeTexture_UnlockBox(texture_3d, 0);

        for (_D3DCUBEMAP_FACES face = D3DCUBEMAP_FACE_POSITIVE_X; face < 6; face = (int16_t)(face + 1))  /* extsh truncation @0x8369B86C */
        {
            D3DCubeTexture_LockRect(texture_cube, face, 0, &locked_rect, nullptr, 0);
            for (int texel = 0; texel < 16; ++texel)
                ((unsigned short *)locked_rect.pBits)[texel] = pattern[texel & 1];
            D3DCubeTexture_UnlockRect(texture_cube, face, 0);
        }
    }

    rasterizer_globals.default_white_hardware_format = white;
    rasterizer_globals.default_2d_hardware_format = texture_2d;
    rasterizer_globals.default_3d_hardware_format = texture_3d;
    rasterizer_globals.default_cm_hardware_format = texture_cube;
}
