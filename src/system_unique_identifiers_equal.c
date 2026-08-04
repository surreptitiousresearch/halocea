/* system_unique_identifiers_equal @0x83762ADC — compares two 16-byte system unique identifiers for
 * equality. The null (all-zero) identifier never compares equal to anything, including itself.
 *
 * DEVIATION: the decompiler declared two extra parameters (a3, a4) and rendered the body around a stack
 * local it named "back_chain" plus a bogus "v14" — these aren't real parameters, they're a 16-byte
 * zero-initialized stack buffer (`std r7,0/8(r9)` zeroing it right before the first comparison loop, per
 * disasm_range). The DB's own prototype confirms only 2 real parameters. The first loop is therefore a
 * null-identifier guard (compares identifier1 against the zeroed buffer); the second loop is the real
 * byte-by-byte comparison against identifier2. */

#include <stdint.h>
#include <string.h>
#include "headers/system_unique_identifier.h"

uint8_t system_unique_identifiers_equal(const system_unique_identifier *identifier1, const system_unique_identifier *identifier2)
{
    static const system_unique_identifier null_identifier = { 0 };

    if ( !memcmp(identifier1, &null_identifier, sizeof(system_unique_identifier)) )
        return 0;

    return !memcmp(identifier1, identifier2, sizeof(system_unique_identifier));
}
