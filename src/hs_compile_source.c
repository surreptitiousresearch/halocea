/* hs_compile_source @0x8372FBC8 — recompile every HaloScript source file in the scenario (permanent pass).
 * Each source file's text is compiled; on error the (single-line) message is surfaced and the result becomes
 * failure. Prints a success message if all compiled, then disposes the compiler. Returns 1 on full success.
 *
 * Deviation: each hs_source_files element is 52 bytes; the tag_data describing the file text is at +32. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_source_file.h"
#include <string.h>

extern void hs_compile_initialize(uint8_t permanent);
extern char *tag_data_get_pointer(const tag_data *data, int offset, int size);
extern void hs_compile(int source_file_size, const char *source_file_data, const char **error_message_pointer, const char **error_source_pointer);
extern void console_printf(uint8_t clear, const char *format, ...);
extern void hs_compile_dispose(void);

uint8_t hs_compile_source(void)
{
    int success = 1;
    scenario *scn = global_scenario;
    hs_compile_initialize(1u);

    for ( int16_t i = 0; i < scn->hs_source_files.count; i = (int16_t)(i + 1) )
    {
        const tag_data *source = &((hs_source_file *)scn->hs_source_files.address)[i].source;
        const char *text = tag_data_get_pointer(source, 0, source->size);
        const char *error_out = 0;
        const char *error_message = 0;
        hs_compile(source->size, text, &error_out, &error_message);
        if ( error_out )
        {
            if ( error_message )
            {
                char *newline = strchr(error_message, '\n');
                if ( newline )
                    *newline = 0;
            }
            success = 0;
        }
    }

    if ( success )
        console_printf(0, "scripts successfully compiled.");
    hs_compile_dispose();
    return success;
}
