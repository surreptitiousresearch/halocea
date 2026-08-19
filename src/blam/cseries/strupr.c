/* strupr @ 0x836F5A10
   Uppercase a C string in place and return it. */
#include <stdint.h>

extern int toupper(int c);

char * strupr(char *string)
{
    for ( char *i = string; *i; ++i )
        *i = toupper((uint8_t)*i);
    return string;
}
