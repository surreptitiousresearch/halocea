#include <stdint.h>
/* ustrnupr calls towlower and its sibling ustrnlwr calls towupper — reproduced faithfully; disasm confirms
 * direct `bl towlower` at this call site, so the naming/behavior mismatch is in the original binary, not a
 * decompiler misread. */

extern unsigned __int16 towlower(unsigned __int16 c);

uint16_t * ustrnupr(uint16_t *string, unsigned int count)
{
    for ( unsigned __int16 *i = string; *i; i++ )
        *i = towlower(*i);
    return string;
}
