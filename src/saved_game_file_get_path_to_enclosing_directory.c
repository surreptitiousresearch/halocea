/* saved_game_file_get_path_to_enclosing_directory @0x837270A8 — release stub: returns an empty path. */
#include <stdint.h>

/* attested uint8_t (was int): all 4 callers clrlwi r3,24 byte-normalize the return */
uint8_t saved_game_file_get_path_to_enclosing_directory(int index, char *full_path)
{
    *full_path = 0;
    return 0;
}
