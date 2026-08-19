/* main_save_current_solo_map @0x8368AF90 — if the given name is a known solo level, persist it as the
 * "last solo map" by writing it to last_solo.txt on the scratch disk. */

#include <stdint.h>

extern int16_t main_get_solo_level_from_name(const char *name);
extern char *system_get_scratch_disk_root(void);
extern int   _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern struct _iobuf *fopen(const char *path, const char *mode);
extern unsigned int fwrite(const void *buffer, unsigned int size, unsigned int count, struct _iobuf *stream);
extern int   fclose(struct _iobuf *stream);

void main_save_current_solo_map(const char *map_name)
{
    if ( main_get_solo_level_from_name(map_name) == -1 )
        return;

    const char *scratch_disk_root = system_get_scratch_disk_root();
    char path[264];
    _snprintf_0(path, 0xFFu, "%s\\last_solo.txt", scratch_disk_root);

    struct _iobuf *file = fopen(path, "w");
    if ( file )
    {
        const char *end = map_name;
        while ( *end++ )
            ;
        fwrite(map_name, 1u, end - map_name, file);
        fclose(file);
    }
}
