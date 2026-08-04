/* rasterizer_dx9_shaders_vshader9_load_from_binary @ 0x83723A28
   Loads the precompiled vertex-shader blob "D:\maps\vsh.bin" and recreates each
   shader. The blob is a sequence of [u32 length][length bytes of shader
   function] records, one per vsf_table slot (in table order). Returns success
   only if all 65 slots were consumed; on a partial/failed load it disposes the
   shaders and raises an error dialog. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/win32_boundary.h"
#include "headers/blam_data_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern void rasterizer_dx9_shaders_vshader9_dispose(void);
extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);

#define VSH9_SRC "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\dx9\\rasterizer_dx9_shaders_vshader9.c"

uint8_t rasterizer_dx9_shaders_vshader9_load_from_binary(void)
{
    void *file = CreateFileA("D:\\maps\\vsh.bin", 0x80000000, 0, 0, 3u, 0x8000000u, 0);
    if ( file == WIN32_INVALID_HANDLE )
        return 0;

    unsigned int size = GetFileSize(file, 0);
    if ( size == 0xFFFFFFFFu )
    {
        CloseHandle(file);
        return 0;
    }

    void *blob = dlMalloc(size, VSH9_SRC, 0xF8);
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

    unsigned int blob_end = (unsigned int)blob + size;
    unsigned int *cursor = (unsigned int *)blob;
    vertex_shader_function *entry = vsf_table;
    int entry_index = 0;

    for ( ; entry_index < 65; ++entry, ++entry_index )
    {
        if ( !entry->filename )
            continue; /* unnamed slot: nothing to load */

        const unsigned int *function = cursor + 1;          /* skip the length dword */
        if ( (unsigned int)function > blob_end )
            break;
        unsigned int *next = (unsigned int *)((char *)function + *cursor);
        if ( (unsigned int)next > blob_end )
            break;

        entry->shader = D3DDevice_CreateVertexShader(function);
        if ( !entry->shader )
            break;
        cursor = next;
    }

    if ( entry_index < 65 )
    {
        rasterizer_dx9_shaders_vshader9_dispose();
        AppendText = "D:\\maps\\vsh.bin";
        DisplayErrorDialog(105, 126, 1);
    }
    dlFree(blob);
    return entry_index == 65;
}
