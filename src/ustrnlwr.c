#include <stdint.h>
/* ustrnlwr calls towupper and its sibling ustrnupr calls towlower — reproduced faithfully; disasm confirms
 * direct `bl towupper` at this call site, so the naming/behavior mismatch is in the original binary, not a
 * decompiler misread. */

extern unsigned __int16 towupper(unsigned __int16 c);

uint16_t * ustrnlwr(uint16_t *string, unsigned int count)
{
    for ( unsigned __int16 *i = string; *i; i++ )
        *i = towupper(*i);
    return string;
}
