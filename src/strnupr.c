/* strnupr @ 0x836F5950
   Uppercase up to n characters of a C string in place; return the string. */
#include <stdint.h>

extern int toupper(int c);

char * strnupr(char *string, int n)
{
    for ( char *i = string; *i; ++i )
    {
        if ( n-- <= 0 )
            break;
        *i = toupper((uint8_t)*i);
    }
    return string;
}
