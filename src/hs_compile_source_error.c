/* hs_compile_source_error @0x8372FB88 — truncates `file_name` at its first newline in place; the other
 * parameters (error_message, error_source, source) are unread here (matches the DB's 4-argument signature —
 * they're presumably consumed by the caller after this normalizes the file name). */

#include <string.h>

void hs_compile_source_error(const char *file_name, const char *error_message, char *error_source,
        const char *source)
{
    if ( file_name )
    {
        char *newline = strchr(file_name, '\n');
        if ( newline )
            *newline = 0;
    }
}
