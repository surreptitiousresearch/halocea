/* strnlwr @ 0x836F59B0
   Lowercase up to n characters of a C string in place; return the string. */

extern int tolower(int c);

char * strnlwr(char *string, int n)
{
    for ( char *i = string; *i; ++i )
    {
        if ( n-- <= 0 )
            break;
        *i = tolower((unsigned __int8)*i);
    }
    return string;
}
