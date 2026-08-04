/* string_has_visible_characters @0x83783444 — true when a wide string contains at least one
 * non-whitespace character. An empty string, or one consisting solely of whitespace, returns false. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */

extern int uisspace(wchar_t c);

uint8_t string_has_visible_characters(const wchar_t *str)
{
    if (!*str)
        return 0;

    const wchar_t *cursor = str;
    while (uisspace(*cursor))
    {
        if (!*++cursor)
            return 0;
    }
    return 1;
}
