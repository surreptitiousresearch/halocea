#include <stdint.h>
#include <stddef.h>   /* wchar_t */

extern uint16_t *saved_game_file_get_display_name(int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

int playlist_profile_get_display_name(int playlist_profile_index, uint16_t *display_name)
{
    unsigned __int16 *name = saved_game_file_get_display_name(playlist_profile_index);

    if ( !name )
        return 0;

    ustrncpy(display_name, (const wchar_t *)name, 0x7Fu);
    display_name[127] = 0;
    return 1;
}
