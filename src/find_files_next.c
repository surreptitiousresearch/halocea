/* find_files_next @0x837E70B8 — advance a recursive directory walk by one entry, returning the next
 * matching file (or directory) into `file` and, optionally, its last-write time into `date`. The walk
 * keeps a per-depth stack of FindFirstFile handles in find_files_globals; each call resumes from the
 * current depth, opening directories on first visit (FindFirstFileA "*.*"), advancing within them
 * (FindNextFileA), and popping back up when a directory is exhausted. "." and ".." are skipped.
 *
 * flags bit 0 (0x1) = descend into subdirectories; bit 1 (0x2) = return directories as results (instead
 * of files). Returns 1 when an entry was produced, 0 when the walk is complete (depth underflows).
 *
 * DEVIATION: the two inlined strcmp("."/"..") byte loops are written as strcmp() calls; the goto-based
 * resume/exhaust control flow is reproduced structurally (a single resume label per loop iteration). */

#include <stdint.h>
#include <string.h>
#include "headers/find_files_globals.h"
#include "headers/find_files_flags.h"
#include "headers/file_reference.h"
#include "headers/file_last_modification_date.h"
#include "headers/blam_data_globals.h"

#define INVALID_HANDLE_VALUE       ((void *)-1)
#define FILE_ATTRIBUTE_DIRECTORY   0x10


extern void *FindFirstFileA(const char *pattern, WIN32_FIND_DATAA *find_data);
extern int   FindNextFileA(void *find_handle, WIN32_FIND_DATAA *find_data);
extern int   CloseHandle(void *handle);
/* memcpy declared by <string.h> */
/* strcmp declared by <string.h> */

extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern void file_path_add_name(char *path, const char *name);
extern void file_path_remove_name(char *path);
extern file_reference *file_reference_create(file_reference *reference, int16_t location);
extern file_reference *file_reference_add_directory(file_reference *reference, const char *directory);
extern file_reference *file_reference_set_name(file_reference *reference, const char *name);

uint8_t find_files_next(file_reference *file, file_last_modification_date *date)
{
    char full_path[368];
    memset(full_path, 0, 256);

    int16_t depth = find_files_globals.depth;
    int level = find_files_globals.depth;
    if (depth < 0)
    {
        find_files_globals.depth = depth;
        return 0;
    }

    while (1)
    {
        void *handle = find_files_globals.handles[level];
        if (handle == INVALID_HANDLE_VALUE)
        {
            /* First visit to this directory: open it. */
            file_location_get_full_path(find_files_globals.location, find_files_globals.path, full_path);
            file_path_add_name(full_path, "*.*");
            handle = FindFirstFileA(full_path, &find_files_globals.data);
            find_files_globals.handles[level] = handle;
            if (handle == INVALID_HANDLE_VALUE)
            {
                file_path_remove_name(find_files_globals.path);
                depth = level - 1;
                goto resume;
            }
        }
        else if (!FindNextFileA(handle, &find_files_globals.data))
        {
            /* Directory exhausted: close and pop up a level. */
            CloseHandle(find_files_globals.handles[level]);
            find_files_globals.handles[level] = INVALID_HANDLE_VALUE;
            file_path_remove_name(find_files_globals.path);
            depth = level - 1;
            goto resume;
        }

        if ((find_files_globals.data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            /* A file: return it unless we are in directories-only mode. */
            if ((find_files_globals.flags & (1u << _find_files_enumerate_directories_bit)) == 0)
            {
                file_reference_create(file, find_files_globals.location);
                file_reference_add_directory(file, find_files_globals.path);
                file_reference_set_name(file, find_files_globals.data.cFileName);
                goto produced;
            }
        }
        else if (strcmp(find_files_globals.data.cFileName, ".") != 0
                 && strcmp(find_files_globals.data.cFileName, "..") != 0)
        {
            char flags = find_files_globals.flags;
            if ((flags & (1 << _find_files_enumerate_directories_bit)) != 0)
            {
                /* Return the directory itself as a result. */
                file_reference_create(file, find_files_globals.location);
                file_reference_add_directory(file, find_files_globals.path);
                file_reference_add_directory(file, find_files_globals.data.cFileName);
                flags = find_files_globals.flags;
            }
            if ((flags & (1 << _find_files_recursive_bit)) != 0)
            {
                /* Descend into the directory next call. */
                if ((flags & (1 << _find_files_enumerate_directories_bit)) == 0)
                {
                    file_path_add_name(find_files_globals.path, find_files_globals.data.cFileName);
                    flags = find_files_globals.flags;
                }
                depth = level + 1;
            }
            if ((flags & (1 << _find_files_enumerate_directories_bit)) != 0)
                goto produced;
        }

    resume:
        level = depth;
        if (depth < 0)
        {
            find_files_globals.depth = depth;
            return 0;
        }
    }

produced:
    if (date)
        /* recovered: *(file_last_modification_date *)&ftLastWriteTime -> copy into date->data */
        memcpy(date->data, &find_files_globals.data.ftLastWriteTime, sizeof(date->data));
    find_files_globals.depth = depth;
    return 1;
}
