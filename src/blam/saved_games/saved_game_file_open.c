/* saved_game_file_open @0x83726B48 — stub in this release: reports failure (returns 0) without opening. */

#include <stdint.h>
#include "headers/file_reference.h"

uint8_t saved_game_file_open(file_reference *saved_game_file, int index)
{
    return 0;
}
