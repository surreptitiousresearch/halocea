/* write_to_error_file @0x83764B70 — append a string to the debug log file (d:\debug.txt) when developer mode
 * is >= 2 and debug-file output is enabled. On the first call it writes a header banner (recursing to log the
 * reference function/address). When `date` is set, each line is prefixed with a local timestamp.
 *
 * Boundary: stdio (fopen-wrapper user_mode_safe_fopen, fprintf, fclose) and the CRT time helpers are externs. */

#include <stdint.h>
#include "headers/developer_mode.h"
#include "headers/error_global_data.h"
#include <stdio.h>
#include <time.h>
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);
extern FILE *user_mode_safe_fopen(const char *filename, const char *how);
/* _time32/_localtime32 come from <time.h>; local externs removed — they clashed with the CRT decls (C4028) */

void write_to_error_file(char *string, uint8_t date)
{
    /* slice candidate _developer_mode_conprint_enabled (=4) rejected: compared value is 2 = _developer_mode_logging_enabled */
    if ( error_globals.developer_mode < (unsigned char)_developer_mode_logging_enabled )
        return;

    if ( first_line )
    {
        char header[1056];
        first_line = 0;
        write_to_error_file("\r\n\r\n", 0);
        write_to_error_file("halo xbox 01.00.01.0563(CACHE) ----------------------------------------------\r\n", 1u);
        sprintf_0(header, "reference function: %s\r\n", "_write_to_error_file");
        write_to_error_file(header, 1u);
        sprintf_0(header, "reference address: %x\r\n", (unsigned int)(uintptr_t)(void *)write_to_error_file);
        write_to_error_file(header, 1u);
    }

    if ( error_globals.output_to_debug_file )
    {
        FILE *file = user_mode_safe_fopen("d:\\debug.txt", "a+b");
        if ( file )
        {
            if ( date )
            {
                __time32_t time_value[4]; /* was int[4]; __time32_t matches the CRT proto (same 32-bit width) */
                _time32(time_value);
                struct tm *local = _localtime32(time_value);
                if ( local )
                    fprintf(file, "%02d.%02d.%02d %02d:%02d:%02d  ",
                            local->tm_mon + 1, local->tm_mday, local->tm_year % 100,
                            local->tm_hour, local->tm_min, local->tm_sec);
                else
                    fprintf(file, "<TIME UNAVAILABLE>  ");
            }
            fprintf(file, "%s", string);
            fclose(file);
        }
    }
}
