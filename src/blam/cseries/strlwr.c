/* strlwr @0x836F5A78 */
/* strlwr 0x836F5A78 — lowercase a C string in place and return it. */

#include <stdint.h>

extern int tolower(int c);

char *strlwr(char *string)
{
    for ( char *i = string; *i; ++i )
        *i = tolower((uint8_t)*i);  /* zero-extension cast models the lbz byte load */
    return string;
}
