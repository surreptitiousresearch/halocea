/* game_state_create_persistent_storage @0x8371BA18 — opens (creating if needed) the persistent-storage
 * directory handle, then immediately closes it — used to force-create the directory without keeping it open. */

#include <stdint.h>

extern void *game_state_open_persistent_storage(const char *directory);
extern int CloseHandle(void *handle);

void game_state_create_persistent_storage(const char *path)
{
    void *handle = (void *)(intptr_t)game_state_open_persistent_storage(path);
    if (handle != (void *)-1)
        CloseHandle(handle);
}
