/* game_engine_playlist_add_selection @0x837524C0 — append a (map, variant) pair to the multiplayer
 * playlist. The variant name is taken from in_variant_name, or derived from the supplied variant's
 * human-readable description. The variant itself is either copied from in_variant or looked up by name.
 * The playlist array is grown by 4 entries when full. Returns 1 on success, 0 if validation/lookup fails.
 *
 * The decompiler's manual NUL-scan + dlMalloc(len_including_terminator) + byte-copy loops are the inlined
 * forms of strlen/strcpy. */

#include <stdint.h>
#include <string.h>
#include "headers/game_engine_playlist_s.h"
#include "headers/game_variant.h"

extern uint8_t game_engine_playlist_verify(const char *map_name, const char *variant_name);
extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);
/* strlen declared by <string.h> */
extern unsigned int ustrlen(const wchar_t *string);
extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);
/* memcpy declared by <string.h> */

static const char SOURCE_FILE[] = "D:\\Projects\\code\\HCEX\\sources\\game\\game_engine.c";

int game_engine_playlist_add_selection(const char *map_name, const char *in_variant_name,
                                       game_variant *in_variant)
{
    if (!game_engine_playlist_verify(map_name, in_variant_name))
        return 0;

    char *variant_name;
    if (in_variant_name)
    {
        variant_name = dlMalloc(strlen(in_variant_name) + 1, SOURCE_FILE, 0x19E1u);
        strcpy(variant_name, in_variant_name);
    }
    else
    {
        unsigned int description_length = ustrlen(in_variant->human_readable_game_description);
        variant_name = dlMalloc(description_length + 1, SOURCE_FILE, 0x19E8u);
        wide_to_ascii(in_variant->human_readable_game_description, variant_name, description_length + 1);
        variant_name[description_length] = 0;
    }

    game_variant variant;
    if (in_variant)
    {
        memcpy(&variant, in_variant, sizeof(variant));
    }
    else if (!game_engine_get_variant_by_name(variant_name, &variant))
    {
        dlFree(variant_name);
        return 0;
    }

    if (!variant_name)
        return 0;

    unsigned int count = game_engine_playlist.playlist_count;
    unsigned int max_count = game_engine_playlist.playlist_max_count;
    game_engine_playlist_item_s *playlist = game_engine_playlist.playlist;
    if (count >= max_count)
    {
        game_engine_playlist.playlist_max_count += 4;
        playlist = dlRealloc(game_engine_playlist.playlist,
                                                            164 * (max_count + 4), SOURCE_FILE, 0x1A02u);
        count = game_engine_playlist.playlist_count;
        game_engine_playlist.playlist = playlist;
    }

    game_engine_playlist_item_s *item = &playlist[count];
    game_engine_playlist.playlist_count = count + 1;
    item->variant_name = variant_name;
    memcpy(&item->variant, &variant, sizeof(item->variant));

    item->map_name = dlMalloc(strlen(map_name) + 1, SOURCE_FILE, 0x1A1Au);
    strcpy(item->map_name, map_name);

    return 1;
}
