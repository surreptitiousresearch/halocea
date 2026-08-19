#include <stdint.h>

extern uint8_t player_ui_get_path_to_local_player_profile_directory(int16_t local_player_index, char *full_path);
extern int DeleteFileA(const char *path);

/* delete_persistent_storage @0x8371B438 — remove the local player's profile save file.
 *
 * DEVIATION: the decompiler threads DeleteFileA's BOOL out through r3. The disasm defines no
 * return value of its own (no `li r3,N` on either path — contrast the encoders in this corpus,
 * where the compiler does emit `li r3,0` for a real 0-return), so the function is void and the
 * r3 at blr is leftover callee state. */

void delete_persistent_storage(void)
{
    char path[264];

    if (player_ui_get_path_to_local_player_profile_directory(0, path))
        DeleteFileA(path);
}
