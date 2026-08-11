/* xbox_set_machine_name @0x83782930 — set the console's network nickname from an ASCII name: convert
 * to wide (max 32 wchars), truncate to 15 characters, and publish via XSetNicknameW. */

#include <wchar.h>
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern int XSetNicknameW(const unsigned short *nickname, int set_offline);

/* name const-qualified 2026-07-30: read-only (deref + widen), hs wrapper passes string-pool arg (C4090) */
void xbox_set_machine_name(const char *name)
{
    unsigned short wide_name[20];
    if ( name && *name )
    {
        if ( ascii_to_wide(name, wide_name, 0x20u) )
        {
            wide_name[15] = 0;
            XSetNicknameW(wide_name, 1);
        }
    }
}
