/* ustrnupr @0x8376D608 */
#include <stdint.h>
/* ustrnupr calls towlower and its sibling ustrnlwr calls towupper — reproduced faithfully; disasm confirms
 * direct `bl towlower` at this call site, so the naming/behavior mismatch is in the original binary, not a
 * decompiler misread. */

extern uint16_t towlower(uint16_t c);

uint16_t * ustrnupr(uint16_t *string, unsigned int count)
{
    for ( uint16_t *i = string; *i; i++ )
        *i = towlower(*i);
    return string;
}
