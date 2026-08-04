/* hcex_tex_new @ 0x823EFED8
   Registers a texture with the engine texture manager. For a named bitmap it
   builds the manager key from the bitmap name + index (and bails to an empty
   key with no streaming flags when texture streaming is disabled); the hardware
   font cache uses the fixed key "hce_hw_font_cache" with flag 96. On success it
   bumps the texture refcount; on failure it falls back to the manager's white
   texture.

   Deviation: the white-texture fallback (txmManager->whiteTex.ptr in the
   binary) is reached through an accessor since the txmMANAGER layout is a
   boundary type and not reversed here. */

#include "../headers/txm_boundary.h"

extern char *hcex_conv_bmp_name(const char *bmp_name, int bmp_index, char *out, int out_size);
extern char *strcpy(char *dst, const char *src);

extern "C" txmTEXTURE *hcex_tex_new(const char *bmp_name, int bmp_index)
{
    char key[136];
    txmMANAGER *manager;
    int flags;

    if ( bmp_name )
    {
        hcex_conv_bmp_name(bmp_name, bmp_index, key, 64);
        manager = txmManager;
        if ( hcex_off_tex_streaming )
            flags = 0;
        else
            flags = 96;
    }
    else
    {
        strcpy(key, "hce_hw_font_cache");
        manager = txmManager;
        flags = 96;
    }

    txmTEXTURE *texture = txmMANAGER__Add(manager, key, flags, ETADDR_CLAMP, ETADDR_CLAMP, 1);
    if ( !texture )
        return txmMANAGER_white_tex(txmManager)->ptr;

    osLockedIncrement(&texture->refCount);
    return texture;
}
