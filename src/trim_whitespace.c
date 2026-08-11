/* trim_whitespace @0x837664E0 — strip leading and trailing whitespace (CRT _SPACE class, plus explicit
 * CR/LF) from a string in place. Trailing whitespace is overwritten with NULs; leading whitespace is
 * likewise NUL'd and *str is advanced past it. */

extern const unsigned short *__pctype_func(void);   /* CRT ctype table accessor */

void trim_whitespace(char **str)
{
    char *end = *str;
    while ( *end++ )
        ;

    /* trim trailing: walk back over whitespace from the terminator, zeroing each */
    char *trailing;
    for ( trailing = end - 1; ; *trailing = 0 )
    {
        int c = *(trailing - 1);
        if ( (__pctype_func()[c] & 8) == 0 && c != 10 && c != 13 )
            break;
        --trailing;
    }

    /* trim leading: zero whitespace and advance the start pointer */
    char *leading;
    for ( leading = *str; ; ++leading )
    {
        int c = *leading;
        if ( (__pctype_func()[c] & 8) == 0 && c != 10 && c != 13 )
            break;
        *leading = 0;
    }
    *str = leading;
}
