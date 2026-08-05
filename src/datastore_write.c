/* datastore_write @0x83768020 — write a named field into a simple flat-file key/value datastore. The store is
 * a single file of 200 fixed 510-byte records (255-byte key + 255-byte value). If the file exists and is
 * exactly the expected size it is loaded and updated in place; otherwise a fresh zeroed buffer is allocated
 * (and any wrong-sized existing file discarded). It scans records for one whose key matches field_name (or the
 * first empty slot), copies the key and the caller's value into it, and writes the whole buffer back. Returns 1
 * if the field was stored, 0 on allocation failure or when the table is full.
 *
 * The record-key match is an inlined strcmp (reproduced faithfully). 102000 == 200 * 510 == 0x18E70. */

#include <stdint.h>
#include "headers/file_reference.h"

extern void *memset(void *dest, int value, unsigned int count);
extern void *memcpy(void *dest, const void *src, unsigned int count);
extern char *strcpy(char *dest, const char *src);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void dlFree(void *ptr);

extern file_reference *file_reference_set_name(file_reference *reference, const char *name);
extern uint8_t file_exists(const file_reference *file);
extern void *file_read_into_memory(file_reference *reference, unsigned int *size);
extern void file_delete(file_reference *reference);
extern uint8_t file_create(file_reference *file);
extern uint8_t file_open(file_reference *file, unsigned int flags);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern uint8_t file_close(file_reference *file);

int datastore_write(const char *file_name, const char *field_name, unsigned int length, const void *data)
{
    int result = 0;
    unsigned int read_size = 0;

    file_reference reference;
    memset(&reference, 0, sizeof(reference));
    *(int16_t *)&reference.data[6] = -1;
    *(int *)reference.data = 1718185071; /* file_reference location magic */
    file_reference_set_name(&reference, file_name);

    char *buffer = 0;
    if ( file_exists(&reference) )
    {
        buffer = file_read_into_memory(&reference, &read_size);
        if ( !buffer )
            file_delete(&reference);
        if ( read_size == 102000 )
        {
            if ( buffer )
                goto have_buffer;
        }
        else
        {
            dlFree(buffer);
            file_delete(&reference);
        }
    }

    buffer = dlMalloc(0x18E70u, "D:\\Projects\\code\\HCEX\\sources\\tag_files\\files.c", 0x1E1u);
    if ( !buffer )
        return result;
    memset(buffer, 0, 0x18E70u);

have_buffer:;
    int record_index = 0;
    uint8_t *record = (uint8_t *)buffer;
    while ( *record )
    {
        const char *key = field_name;
        uint8_t *record_char = record;
        int diff;
        do
        {
            int record_byte = *record_char;
            diff = record_byte - *(uint8_t *)key;
            if ( record_byte == 0 )
                break;
            ++record_char;
            ++key;
        }
        while ( !diff );
        if ( !diff )
            break;
        ++record_index;
        record += 510;
        if ( record_index >= 200 )
            goto write_file;
    }

    {
        char *slot = &buffer[510 * record_index];
        strcpy(slot, field_name);
        memcpy(slot + 255, data, length);
        result = 1;
    }

write_file:
    if ( !file_exists(&reference) )
        file_create(&reference);
    if ( file_open(&reference, 2u) )
    {
        file_write(&reference, 0x18E70u, buffer);
        file_close(&reference);
    }
    dlFree(buffer);
    return result;
}
