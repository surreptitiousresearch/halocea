/* is_string_all_digits @0x83766330 — true if every character is a digit or '-'. Empty string returns 1.
 * Deviation: digit class read via isdigit instead of the inlined __pctype_func()[c]&4, per hs_parse_integer.c. */

#include <stdint.h>
#include <ctype.h>

uint8_t is_string_all_digits(const char *str)
{
    for ( const char *i = str; i && *i; ++i )
    {
        if ( !isdigit((unsigned char)*i) && *i != '-' )
            return 0;
    }
    return 1;
}
