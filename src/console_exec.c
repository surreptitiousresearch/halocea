/* console_exec @0x837228A8 — execute a console script file: read it line by line, skip blank/comment lines
 * (';', '#', '//'), push each command onto the 8-deep command history ring, and if the command's leading
 * token names a known HaloScript function, compile-and-evaluate the line; otherwise report it cannot run.
 * Returns 1 if the file opened, 0 otherwise.
 *
 * Deviations:
 *   - The history write is a raw pointer-arithmetic byte copy landing on console_globals.previous_commands
 *     [newest_previous_command_index]; reproduced as a strcpy of the full line.
 *   - The token search walks hs_tokens_enumerate's output backwards looking for a case-insensitive
 *     match to the command name. */

#include <stdint.h>
#include "headers/console_globals.h"
#include <stdio.h>
#include <string.h>
#include "headers/blam_data_globals.h"

extern char *strtok(char *str, const char *delim);
extern char *strncpy(char *, const char *, unsigned __int64);
extern uint16_t get_console_test_flags(uint16_t extra_flags);
extern int16_t hs_tokens_enumerate(const char *substring, int type_flags, const char **results, int16_t maximum_count, uint16_t console_flags_to_test);
extern int stricmp(const char *a, const char *b);
extern int hs_compile_and_evaluate(const char *expression);
extern void console_printf_only_if_open(uint8_t clear, const char *format, ...);

uint8_t console_exec(char *filename)
{
    FILE *file = fopen(filename, "r");
    if ( !file )
        return 0;

    optionNoVideo = 1;

    char line[208];
    while ( fgets(line, 199, file) )
    {
        strtok(line, "\r\n\t");

        if ( line[0] == ';' || line[0] == '#' || (line[0] == '/' && line[1] == '/') )
            continue;

        char command[256];
        strncpy(command, line, 0xFFu);
        command[255] = 0;
        char *space = strchr(command, ' ');
        if ( space )
            *space = 0;

        /* push the full line onto the history ring */
        console_globals.newest_previous_command_index = (console_globals.newest_previous_command_index + 1) % 8;
        strcpy(console_globals.previous_commands[console_globals.newest_previous_command_index], line);

        int count = console_globals.previous_command_count + 1;
        if ( count > 8 )
            count = 8;
        console_globals.previous_command_count = count;
        console_globals.selected_previous_command_index = -1;

        /* 0x2000: an extra hs symbol-category bit ORed into the enumeration mask; no DB enum
         * found for the hs_tokens_enumerate category flags (searched types_enum_values for
         * value 8192 / hs* token/script enums, src/headers/hs_*). Left raw. */
        unsigned __int16 test_flags = get_console_test_flags(0x2000u);
        char is_known_function = 0;
        const char *tokens[256];
        int token_count = hs_tokens_enumerate(command, '(', tokens, 256, test_flags);
        if ( token_count > 0 )
        {
            while ( stricmp(command, tokens[token_count - 1]) )
            {
                token_count = (__int16)(token_count - 1);
                if ( token_count <= 0 )
                    break;
            }
            if ( token_count > 0 )
                is_known_function = 1;
        }

        if ( is_known_function )
        {
            hs_console_mode = 1;
            hs_compile_and_evaluate(line);
            hs_console_mode = 0;
        }
        else
        {
            console_printf_only_if_open(0, "Requested function \"%s\" cannot be executed now.", command);
        }
    }

    fclose(file);
    return 1;
}
