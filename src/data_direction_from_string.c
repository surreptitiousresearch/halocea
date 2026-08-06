/* data_direction_from_string @0x83763CB8 — DEVIATION: the decompiler expressed the table-end test against
 * an unrelated adjacent string literal ("%s %s"). Per the names table, data_direction_strings sits at
 * 0x8211DEE4 and that string at 0x8211DEEC — a gap of 8 bytes, i.e. exactly 2 entries — so the scan is
 * reproduced as an index-bounded search (same fix class as create_mutex.c). */

#define DATA_DIRECTION_STRING_COUNT 2

/* DEVIATION: the local extern typed the table as `const char *[]` (mutable pointers); the object is in
 * .rdata, so the pointers are const too. Declared canonically by headers/blam_data_globals.h
 * (const char *const [2], def src/data/data_direction_strings.c). */
#include "headers/blam_data_globals.h"

extern int stricmp(const char *string1, const char *string2);

int data_direction_from_string(const char *direction_string)
{
    for ( int i = 0; i < DATA_DIRECTION_STRING_COUNT; i++ )
    {
        if ( !stricmp(direction_string, data_direction_strings[i]) )
            return i;
    }
    return -1;
}
