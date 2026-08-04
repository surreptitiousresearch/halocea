#include <stdint.h>
/* cached_map_file_get_path @0x83753EE0 — builds the on-disk path of a scratch-disk cache-copy file
 * ("<scratch_disk_root>\cache%03d.map") for the given cache map file index. */

extern char *system_get_scratch_disk_root(void);
extern int sprintf_0(char *string, const char *format, ...);

void cached_map_file_get_path(int16_t map_file_index, char *path)
{
    const char *scratch_disk_root = system_get_scratch_disk_root();
    sprintf_0(path, "%s\\cache%03d.map", scratch_disk_root, map_file_index);
}
