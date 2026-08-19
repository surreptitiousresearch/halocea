/* rasterizer_dx9_decrypt_file @0x8369D640 — hcex bridge: open an encrypted shader/asset file, read the
 * whole thing into a heap buffer, decrypt it in place via rasterizer_dx9_decrypt_buffer, and hand the
 * caller the decrypted buffer + its size. Returns 1 on success (out params set), 0 on any failure
 * (out params left as null/0 and the buffer freed / handle closed). */

#include <stdint.h>
#include "headers/win32_boundary.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
extern uint8_t rasterizer_dx9_decrypt_buffer(uint8_t *buffer, unsigned int bufferSize);

int rasterizer_dx9_decrypt_file(char *filename, uint8_t **resultBuffer, unsigned int *resultSize)
{
    void *fileHandle;
    unsigned int fileSize;
    void *buffer;
    unsigned int bytesRead;

    *resultBuffer = 0;
    *resultSize = 0;

    fileHandle = CreateFileA(filename, 0x80000000, 0, 0, 3u, 0x8000000u, 0);
    if ( fileHandle == WIN32_INVALID_HANDLE )
        return 0;

    fileSize = GetFileSize(fileHandle, 0);
    if ( fileSize == (unsigned int)-1 )
    {
        CloseHandle(fileHandle);
        return 0;
    }

    buffer = dlMalloc(fileSize, "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\dx9\\rasterizer_dx9.c", 0xE6Du);
    if ( !buffer )
    {
        CloseHandle(fileHandle);
        return 0;
    }

    if ( !ReadFile(fileHandle, buffer, fileSize, &bytesRead, 0) )
    {
        dlFree(buffer);
        CloseHandle(fileHandle);
        return 0;
    }

    CloseHandle(fileHandle);

    if ( !rasterizer_dx9_decrypt_buffer((unsigned char *)buffer, fileSize) )
    {
        dlFree(buffer);
        return 0;
    }

    *resultBuffer = (uint8_t *)buffer;
    *resultSize = fileSize;
    return 1;
}
