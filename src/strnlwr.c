/* strnlwr @ 0x836F59B0
   Lowercase up to n characters of a C string in place; return the string. */
#include <stdint.h>

extern int tolower(int c);

char * strnlwr(char *string, int n)
{
    for ( char *i = string; *i; ++i )
    {
        if ( n-- <= 0 )
            break;
        *i = tolower((uint8_t)*i);
    }
    return string;
}
