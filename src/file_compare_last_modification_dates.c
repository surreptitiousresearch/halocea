/* file_compare_last_modification_dates @0x837E6128 — byte-wise memcmp of two 8-byte timestamps, stopping at
 * the first differing byte (returns that signed byte difference), or 0 if all 8 bytes match. */

#include "headers/file_last_modification_date.h"

int file_compare_last_modification_dates(file_last_modification_date *date1, file_last_modification_date *date2)
{
    int result = 0;

    for (int i = 0; i < 8; i++)
    {
        /* recovered: (unsigned char *)date1[i] -> date1->data[i]; keep unsigned compare semantics */
        unsigned char a = (unsigned char)date1->data[i];
        unsigned char b = (unsigned char)date2->data[i];
        result = a - b;
        if (a != b)
            break;
    }

    return result;
}
