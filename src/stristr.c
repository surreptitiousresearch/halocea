/* stristr @ 0x836F58B8
   Case-insensitive substring search; returns pointer to first match or NULL. */

extern int strnicmp(const char *a, const char *b, unsigned int count);

char * stristr(const char *haystack, const char *needle)
{
    const char *needle_rest;
    int first;
    const char *scan;
    unsigned int rest_length;
    int c;

    needle_rest = needle + 1;
    first = *needle;
    if ( !*needle )
        return (char *)haystack;

    scan = needle + 1;
    while ( *scan++ )
        ;
    rest_length = (unsigned int)(scan - needle_rest - 1);

    while ( 1 )
    {
        c = *haystack++;
        if ( !c )
            break;
        if ( c == first && !strnicmp(haystack, needle_rest, rest_length) )
            return (char *)--haystack;
    }
    return nullptr;
}
