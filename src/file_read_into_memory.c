/* file_read_into_memory @0x83767BD8 — open `reference`, allocate a buffer for its entire contents, read it in,
 * and return the buffer (with the byte count in *size). Returns null on open/alloc/read failure. */

#include <stdint.h>
#include "headers/file_reference.h"

extern uint8_t file_open(file_reference *file, unsigned int flags);
extern unsigned int file_get_eof(const file_reference *file);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern uint8_t file_read(const file_reference *file, unsigned int count, void *buffer);
extern void dlFree(void *ptr);
extern uint8_t file_close(file_reference *file);

void *file_read_into_memory(file_reference *reference, unsigned int *size)
{
    void *buffer = nullptr;
    if ( file_open(reference, 1u) )
    {
        unsigned int eof = file_get_eof(reference);
        *size = eof;
        buffer = dlMalloc(eof, "D:\\Projects\\code\\HCEX\\sources\\tag_files\\files.c", 0x12Cu);
        if ( buffer && !file_read(reference, *size, buffer) )
        {
            dlFree(buffer);
            buffer = nullptr;
        }
        file_close(reference);
    }
    return buffer;
}
