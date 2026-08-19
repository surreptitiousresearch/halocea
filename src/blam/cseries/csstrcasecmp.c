/* csstrcasecmp @0x836F5BA0 — cseries case-insensitive string compare. Faithful quirks: each (signed) char
 * is widened and passed through the wide-character towlower (so bytes >= 0x80 sign-extend before the
 * lookup), and the s1 cursor is maintained as a pointer difference against the advancing s2 cursor, exactly
 * as compiled. Returns <0 / 0 / >0 like strcasecmp. */
#include <stdint.h>

extern uint16_t towlower(uint16_t c);

int csstrcasecmp(const char *s1, const char *s2)
{
    int c1 = towlower(*s1);
    int c2 = towlower(*s2);
    if ( !c1 )
        return (c2 == 0) - 1;

    int s1_minus_s2 = s1 - s2;
    while ( 1 )
    {
        if ( !c2 )
            return c1 != 0;
        if ( c1 != c2 )
            break;
        ++s2;
        c1 = towlower(s2[s1_minus_s2]);
        c2 = towlower(*s2);
        if ( !c1 )
            return (c2 == 0) - 1;
    }
    return c1 <= c2 ? -1 : 1;
}
