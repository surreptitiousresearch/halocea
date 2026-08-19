/* playlist_profile_create_default_profiles_on_disk @0x837821E0 — write the built-in default multiplayer game
 * variants to the memory unit as playlist profiles. Requires the "ui\default_multiplayer_game_setting_names"
 * unicode string list to be loaded; for each entry in default_variant_building_functions[] it builds the
 * variant, names it from the matching localized string, stamps the profile index, checksums it, and writes a
 * 512-byte "blam.lst" record into saved-game slot 6's per-index directory. Successful writes bump the
 * default-profile count. Finishes by notifying that memory-unit contents changed.
 *
 * DEVIATION: saved_game_files_notify_memory_units_changed() takes no argument and returns void (disasm
 * 0x83782394 sets up no arg register; DB proto confirms); Hex-Rays passed a stale `v8` and returned its value.
 * The record buffer (u16[76]=152 bytes) and checksum buffer (u8[360]) are contiguous on the stack and together
 * form the 512-byte on-disk record that file_write emits — reproduced as declared.
 * DEVIATION: the decompiler gave this an `int` return threaded out of the tail call; no path writes r3
 * explicitly (the early exit merely leaves tag_loaded's -1 in r3) and the sole caller @0x83782658 ignores
 * r3 and declares it void. Declared void. */

#include <stdint.h>
#include <string.h>
#include "headers/game_variant.h"
#include "headers/file_reference.h"
#include "headers/playlist_profile_globals.h"
#include "headers/saved_game_files_path_type.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern char *saved_game_files_get_path(int16_t path_type);
extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern void directory_create_or_delete_contents(const char *directory_name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum);
extern file_reference *file_reference_create_from_path_absolute(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_create(file_reference *file);
extern uint8_t file_open(file_reference *file, unsigned int flags);
extern uint8_t file_set_position(const file_reference *file, unsigned int position);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern uint8_t file_close(file_reference *file);
extern void saved_game_files_notify_memory_units_changed(void);
/* memcpy declared by <string.h> */

extern game_variant *(*default_variant_building_functions[])(game_variant *);
extern int first_time_0; /* end-of-array sentinel immediately following default_variant_building_functions */

void playlist_profile_create_default_profiles_on_disk(void)
{
    int string_list_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\default_multiplayer_game_setting_names");
    if ( string_list_tag_index == -1 )
        return;

    int profile_index = 0;
    game_variant *(**builder)(game_variant *) = default_variant_building_functions;
    file_reference file;
    char path_buffer[256];
    uint16_t record_buffer[76];
    uint8_t checksum_buffer[360];
    game_variant variant_copy;
    game_variant variant_scratch;

    do
    {
        uint8_t file_written = 0;

        game_variant *built_variant = (*builder)(&variant_scratch);
        memcpy(&variant_copy, built_variant, sizeof(variant_copy));

        const char *slot_path = saved_game_files_get_path(_saved_game_files_path_type_default_playlist_path);
        _snprintf_0(path_buffer, 0xFFu, "%s\\%02d", slot_path, profile_index);
        path_buffer[255] = 0;
        directory_create_or_delete_contents(path_buffer);
        strncat(path_buffer, "\\blam.lst", 0xFFu);
        path_buffer[255] = 0;

        const wchar_t *profile_name = (const wchar_t *)unicode_string_list_get_string(string_list_tag_index, profile_index);
        memcpy(record_buffer, &variant_copy, sizeof(record_buffer));
        ustrncpy(record_buffer, profile_name, 0x17u);
        record_buffer[23] = 0;
        record_buffer[74] |= (uint16_t)(profile_index << 8);
        saved_game_file_generate_checksum(record_buffer, 0x98u, checksum_buffer);

        if ( file_reference_create_from_path_absolute(&file, path_buffer, 0) )
        {
            if ( file_create(&file) )
            {
                if ( file_open(&file, 2u) )
                {
                    if ( file_set_position(&file, 0) )
                    {
                        file_written = file_write(&file, 0x200u, record_buffer);
                        file_close(&file);
                    }
                }
            }
        }

        if ( file_written == 1 )
            ++playlist_profile_globals.number_of_default_profiles;

        ++builder;
        ++profile_index;
    }
    while ( (uintptr_t)builder < (uintptr_t)&first_time_0 );

    saved_game_files_notify_memory_units_changed();
}
