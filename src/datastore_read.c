/* datastore_read @0x83767EC0 — read a named field's raw value out of a simple flat-file "datastore":
 * loads the whole file into memory (verifying it's the expected 102000-byte format), then scans up to 200
 * fixed 510-byte records for one whose
 * leading NUL-terminated name matches field_name; if found (and the record isn't itself empty), copies
 * `length` bytes starting 255 bytes into that record into `data`. Returns whether a matching field was
 * found and copied. */

#include <stdint.h>
#include "headers/datastore_entry.h"
#include "headers/file_reference.h"

extern void *memset(void *destination, int value, unsigned int size);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern file_reference *file_reference_set_name(file_reference *reference, const char *name);
extern uint8_t file_exists(const file_reference *file);
extern void *file_read_into_memory(file_reference *reference, unsigned int *size);
extern uint8_t file_delete(file_reference *file);
extern void dlFree(void *ptr);

uint8_t datastore_read(const char *file_name, const char *field_name, int length, void *data)
{
    uint8_t found = 0;

    unsigned int file_size = 0;
    file_reference reference;
    memset(&reference, 0, sizeof(reference));
    *(int16_t *)&reference.data[6] = -1;
    *(int *)reference.data = 1718185071; /* file_reference location magic */
    file_reference_set_name(&reference, file_name);

    if ( !file_exists(&reference) )
        return found;

    char *contents = file_read_into_memory(&reference, &file_size);
    if ( !contents )
        file_delete(&reference);

    if ( file_size != 102000 )
    {
        dlFree(contents);
        file_delete(&reference);
        return found;
    }

    if ( !contents )
        return found;

    /* 102000 == 200 * sizeof(datastore_entry) — the file is exactly 200 fixed slots. */
    const datastore_entry *entries = (const datastore_entry *)contents;
    int record_index = 0;
    const unsigned char *record;

    for ( record = (const unsigned char *)entries[0].name; ; record += sizeof(datastore_entry) )
    {
        const char *name = field_name;
        const unsigned char *record_name = record;
        int diff;

        do
        {
            diff = *record_name - (unsigned char)*name;
            if ( !*record_name )
                break;
            ++record_name;
            ++name;
        }
        while ( !diff );

        if ( !diff )
            break;

        if ( !*record )
            goto found_empty;

        if ( ++record_index >= 200 )
        {
            dlFree(contents);
            return found;
        }
    }

    memcpy(data, entries[record_index].data, length);
    found = 1;

found_empty:
    dlFree(contents);

    return found;
}
