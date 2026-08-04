/* data_type_from_string @0x83763C50 — DEVIATION: the decompiler expressed the table-end test against the
 * adjacent data_direction_strings symbol. Per the names table, data_type_strings sits at 0x8211DEDC and
 * data_direction_strings at 0x8211DEE4 — a gap of 8 bytes, i.e. exactly 2 entries — so the scan is
 * reproduced as an index-bounded search (same fix class as create_mutex.c). */

#define DATA_TYPE_STRING_COUNT 2

extern const char *data_type_strings[DATA_TYPE_STRING_COUNT];
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
