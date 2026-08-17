/* virtual_keyboard_playlist_name_unique @0x83783490 */
#include <stdint.h>
#include "headers/game_variant.h"

#include "headers/game_variant.h"
#include "headers/game_variant.h"
extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);
extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);

BOOL virtual_keyboard_playlist_name_unique(wchar_t *playlist_name)
{
    char ascii_name[40];

    wide_to_ascii(playlist_name, ascii_name, 0x20u);
    return game_engine_get_variant_by_name(ascii_name, 0) == 0;
}
