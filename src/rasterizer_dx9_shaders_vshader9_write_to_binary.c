/* rasterizer_dx9_shaders_vshader9_write_to_binary @ 0x83723958
   Dumps every named vertex shader to the binary blob "D:\maps\vsh.bin" (dev
   tooling path). Stops on the first failure. Iterates the 65-entry vsf_table
   (the binary bounds the loop with the adjacent keyboard_auxbutton_mapping). */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/win32_boundary.h"

extern uint8_t rasterizer_dx9_shaders_vshader9_write_effect(int shaderIndex, void *fileHandle);

/* attest: uint8_t return — the succeeded flag is kept byte-normalized (clrlwi r31 @
   0x837239F8) and returned via mr r3,r31 (or li r3,0 on CreateFileA failure); the
   sole caller ignores the value. */
uint8_t rasterizer_dx9_shaders_vshader9_write_to_binary(void)
{
    void *file = CreateFileA("D:\\maps\\vsh.bin", 0x40000000u, 0, 0, 2u, 0x8000000u, 0);
    if ( file == WIN32_INVALID_HANDLE )
        return 0;

    uint8_t succeeded = 1;
    for ( int index = 0; index < 65 && succeeded; ++index )
    {
        if ( vsf_table[index].filename )
        {
            if ( !rasterizer_dx9_shaders_vshader9_write_effect(index, file) )
                succeeded = 0;
        }
    }
    CloseHandle(file);
    return succeeded;
}
