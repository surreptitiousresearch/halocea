/* cache_files_open_cache_file @0x837540B0 */
#include <stdint.h>

void *CreateFileA(const char *path, uint32_t access, int share_read, void *security,
                  uint32_t creation, uint32_t flags, void *template_file);

void *cache_files_open_cache_file(const char *path, uint8_t writable)
{
    uint32_t access;

    access = writable ? 0xC0000000u : 0x80000000u;
    return CreateFileA(path, access, writable == 0, 0, 4u, 0x48000080u, 0);
}
