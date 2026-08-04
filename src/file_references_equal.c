/* file_references_equal @0x83767B28 — two file_references are equal when their type/location word at
 * +6 matches and their NUL-terminated path strings (starting at +8) are byte-identical. */

#include "headers/file_reference.h"

int file_references_equal(const file_reference *reference0, const file_reference *reference1)
{
    if (*(unsigned __int16 *)&reference0->data[6] != *(unsigned __int16 *)&reference1->data[6])
        return 0;

    const char *path0 = &reference0->data[8];
    const char *path1 = &reference1->data[8];

    while (*path0 == *path1)
    {
        if (!*path0)
            return 1;
        path0++;
        path1++;
    }
    return 0;
}
