/* rasterizer_dx9_shaders_load_from_binary @ 0x836A21A8
   Loads the precompiled pixel-shader blob "D:\maps\fx.bin". The blob is a
   sequence of [u32 length][length bytes] records: 123 effect records (each
   passed to create_effect_from_binary + init_effect), followed by the two
   chicago fixed-function pixel shaders and the decal pixel shader. Returns
   success only if every record loaded; on failure it unloads all effects.

   The trailing chicago/decal section is reproduced with raw byte-offset walking
   matching the binary (length-prefixed records consumed in place). */

#include <stdint.h>
#include <string.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/chicago_shaders.h"
#include "headers/win32_boundary.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern uint8_t rasterizer_dx9_shaders_create_effect_from_binary(unsigned int shaderIndex, uint8_t *buffer, unsigned int bufferSize);
extern uint8_t rasterizer_dx9_shaders_init_effect(int index);
extern void rasterizer_dx9_shaders_unload_effects(void);

#define SHADERS_SRC "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\dx9\\rasterizer_dx9_shaders.c"

uint8_t rasterizer_dx9_shaders_load_from_binary(void)
{
    void *file = CreateFileA("D:\\maps\\fx.bin", 0x80000000, 0, 0, 3u, 0x8000000u, 0);
    if ( file == WIN32_INVALID_HANDLE )
        return 0;

    unsigned int size = GetFileSize(file, 0);
    if ( size == 0xFFFFFFFFu )
    {
        CloseHandle(file);
        return 0;
    }

    void *blob = dlMalloc(size, SHADERS_SRC, 0x533);
    if ( !blob )
    {
        CloseHandle(file);
        return 0;
    }

    unsigned int bytes_read;
    if ( !ReadFile(file, blob, size, &bytes_read, 0) )
    {
        dlFree(blob);
        CloseHandle(file);
        return 0;
    }
    CloseHandle(file);

    uintptr_t blob_end = (uintptr_t)blob + size;
    unsigned int *cursor = (unsigned int *)blob;
    unsigned char *record_end = 0;
    int index = 0;

    for ( ; index < 123; ++index )
    {
        unsigned char *data = (unsigned char *)(cursor + 1);  /* skip length dword */
        if ( (uintptr_t)data > blob_end )
            break;
        record_end = &data[*cursor];
        if ( (uintptr_t)record_end > blob_end )
            break;
        if ( !rasterizer_dx9_shaders_create_effect_from_binary(index, data, *cursor) )
            break;
        if ( !rasterizer_dx9_shaders_init_effect(index) )
            break;
        cursor = (unsigned int *)record_end;
    }

    if ( index != 123 )
        rasterizer_dx9_shaders_unload_effects(); /* binary unloads on the failing record */

    int succeeded = 0;
    if ( index == 123 )
    {
        /* trailing chicago fixed-function + decal pixel shaders */
        memset(chicago_shader_table, 0, 0x48u);
        /* chicago_shader_table[0/1].is_cube (+0x14) */
        chicago_shader_table[0].is_cube = 0;
        chicago_shader_table[1].is_cube = 1;

        unsigned char *chicago0_data = record_end + 4;          /* skip length */
        unsigned int chicago0_len = *(unsigned int *)record_end;
        /* chicago_shader_table[0/1].shader (+0x20) */
        chicago_shader_table[0].shader = D3DDevice_CreatePixelShader((const unsigned int *)record_end + 1);
        if ( chicago_shader_table[0].shader != 0 )
        {
            ++chicago_shader_table_size;
            unsigned char *chicago1 = &chicago0_data[chicago0_len];
            unsigned int chicago1_len = *(unsigned int *)chicago1;
            unsigned char *chicago1_data = chicago1 + 4;
            chicago_shader_table[1].shader = D3DDevice_CreatePixelShader((const unsigned int *)chicago1 + 1);
            if ( chicago_shader_table[1].shader != 0 )
            {
                ++chicago_shader_table_size;
                decal_shader = D3DDevice_CreatePixelShader((const unsigned int *)&chicago1_data[chicago1_len + 4]);
                succeeded = decal_shader != 0;
            }
        }
    }

    dlFree(blob);
    if ( index != 123 )
        return 0;
    return succeeded ? 1 : 0;
}
