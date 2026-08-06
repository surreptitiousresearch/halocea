/* data_type_from_string @0x83763C50 — DEVIATION: the decompiler expressed the table-end test against the
 * adjacent data_direction_strings symbol. Per the names table, data_type_strings sits at 0x8211DEDC and
 * data_direction_strings at 0x8211DEE4 — a gap of 8 bytes, i.e. exactly 2 entries — so the scan is
 * reproduced as an index-bounded search (same fix class as create_mutex.c). */

#define DATA_TYPE_STRING_COUNT 2

/* DEVIATION: the local extern typed the table as `const char *[]` (mutable pointers); the object is in
 * .rdata, so the pointers are const too. Declared canonically by headers/blam_data_globals.h
 * (const char *const [2], def src/data/data_type_strings.c). */
#include "headers/blam_data_globals.h"

extern int stricmp(const char *string1, const char *string2);

int data_type_from_string(const char *type_string)
{
    for ( int i = 0; i < DATA_TYPE_STRING_COUNT; i++ )
    {
        if ( !stricmp(type_string, data_type_strings[i]) )
            return i;
    }
    return -1;
}
