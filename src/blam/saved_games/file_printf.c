/* file_printf @0x83767C68 — format a message and append it to an open file, then truncate the file at the
 * new end (so the file always ends exactly after the last write, discarding any stale trailing bytes from a
 * previous longer write).
 *
 * DEVIATION: the decompiler's manual a3-a11/va_arg register shuffling is PowerPC varargs ABI noise (the
 * "local variable allocation has failed" warning is this codebase's usual symptom of it) — reproduced with a
 * standard va_list, matching the established console_printf.c idiom. */

#include <stdint.h>
#include <stdarg.h>
#include "headers/file_reference.h"

extern int vsprintf_0(char *buffer, const char *format, va_list args);
extern uint8_t file_write(const file_reference *file, unsigned int count, const void *buffer);
extern unsigned int file_get_position(const file_reference *file);
extern uint8_t file_set_eof(const file_reference *file, unsigned int position);

void file_printf(file_reference *file, char *format, ...)
{
    if ( !format )
        return;

    char buffer[1024];
    va_list args;

    va_start(args, format);
    vsprintf_0(buffer, format, args);
    va_end(args);

    unsigned int length = 0;
    while ( buffer[length] )
        length++;

    file_write(file, length, buffer);
    unsigned int position = file_get_position(file);
    file_set_eof(file, position);
}
