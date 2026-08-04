/* main_get_solo_level_from_name @0x83689528 — map a map name to its solo campaign level index (0..9) by
 * lowercasing it and matching the known scenario stubs; returns -1 if it is not a campaign level. */

#include <stdint.h>
extern void *memset(void *dst, int c, unsigned int n);
extern char *strncpy(char *dst, const char *src, unsigned int n);
extern char *strlwr(char *string);
extern char *strstr(const char *haystack, const char *needle);

int16_t main_get_solo_level_from_name(const char *name)
{
    char lower[136];
    memset(lower, 0, 128);
    strncpy(lower, name, 0x7Fu);
    lower[127] = 0;
    strlwr(lower);

    if ( strstr(lower, "a10") ) return 0;
    if ( strstr(lower, "a30") ) return 1;
    if ( strstr(lower, "a50") ) return 2;
    if ( strstr(lower, "b30") ) return 3;
    if ( strstr(lower, "b40") ) return 4;
    if ( strstr(lower, "c10") ) return 5;
    if ( strstr(lower, "c20") ) return 6;
    if ( strstr(lower, "c40") ) return 7;
    if ( strstr(lower, "d20") ) return 8;
    return strstr(lower, "d40") ? 9 : -1;
}
