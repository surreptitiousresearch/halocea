/* file_reference_copy @0x83767978 — bytewise copy of a file_reference. The copy length (0x108 = 264 bytes,
 * disasm-confirmed literal) is 4 bytes short of file_reference's full 268-byte DB size — presumably the
 * struct's trailing 4 bytes are padding never worth copying. */

#include "headers/file_reference.h"
#include <string.h>

file_reference * file_reference_copy(file_reference *destination, const file_reference *source)
{
    memcpy(destination, source, 0x108);
    return destination;
}
