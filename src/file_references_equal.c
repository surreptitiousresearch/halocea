/* file_references_equal @0x83767B28 — two file_references are equal when their type/location word at
 * +6 matches and their NUL-terminated path strings (starting at +8) are byte-identical. */

#include <stdint.h>
#include "headers/file_reference.h"

int file_references_equal(const file_reference *reference0, const file_reference *reference1)
{
    if (reference0->info.location != reference1->info.location)
        return 0;

    const char *path0 = reference0->info.path;
    const char *path1 = reference1->info.path;

    while (*path0 == *path1)
    {
        if (!*path0)
            return 1;
        path0++;
        path1++;
    }
    return 0;
}
