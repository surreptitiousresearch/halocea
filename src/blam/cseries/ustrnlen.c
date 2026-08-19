/* ustrnlen @0x8376D508 — bounded UTF-16 string length: number of wchars before the first NUL,
 * capped at count. */

#include <wchar.h>

unsigned int ustrnlen(const wchar_t *string, unsigned int count)
{
    unsigned int length = 0;

    if ( count )
    {
        const wchar_t *p = string;
        do
        {
            if ( !*p++ )
                break;
            ++length;
        }
        while ( length < count );
    }
    return length;
}
