/* console_process_command @ 0x83722330 — record a typed command in the history ring and,
 * unless it is a comment (starting with ';', '#' or '//'), look it up among the available
 * hs script functions/globals and compile-and-evaluate it. Returns 0 for comments or a
 * command that cannot run now, otherwise the evaluation result.
 *
 * The decompiler reached the history slot by raw pointer math off
 * &input_state.edit.selection_start_index; reproduced here via the previous_commands ring. */

#include <stdint.h>
#include "headers/console_globals.h"
#include "headers/blam_data_globals.h"

extern char  *strncpy(char *dst, const char *src, unsigned int n);
extern char  *strchr(const char *s, int c);
extern int    stricmp(const char *a, const char *b);
extern int16_t hs_tokens_enumerate(const char *substring, int type_flags, const char **results, int16_t maximum_count, uint16_t console_flags_to_test);
extern uint16_t get_console_test_flags(uint16_t extra_flags);
extern int hs_compile_and_evaluate(const char *expression);
extern void console_printf_only_if_open(uint8_t clear, const char *format, ...);


int console_process_command(const char *command, uint16_t extra_flags)
{
    char first = *command;
    int result = 1;
    char short_name[256];
    const char *tokens[268];
    char *space;
    char *dst;
    const char *src;
    char ch;
    int history_count;
    uint16_t test_flags;
    int matched;
    int token_index;

    if ( first == ';' || first == '#' || (first == '/' && command[1] == '/') )
        result = 0;

    if ( !(unsigned char)result )
        return result;

    /* the command's first word becomes the lookup name */
    strncpy(short_name, command, 0xFFu);
    short_name[255] = 0;
    space = strchr(short_name, ' ');
    if ( space )
        *space = 0;

    /* push the full command into the previous-command ring */
    console_globals.newest_previous_command_index =
        (console_globals.newest_previous_command_index + 1) % 8;
    dst = console_globals.previous_commands[console_globals.newest_previous_command_index];
    src = command;
    do
    {
        ch = *src;
        *dst++ = *src++;
    }
    while ( ch );

    history_count = console_globals.previous_command_count + 1;
    if ( history_count > 8 )
        history_count = 8;
    console_globals.previous_command_count = history_count;
    console_globals.selected_previous_command_index = -1;

    test_flags = get_console_test_flags(extra_flags);
    matched = 0;
    token_index = hs_tokens_enumerate(short_name, '(', tokens, 256, test_flags);
    if ( token_index > 0 )
    {
        while ( stricmp(short_name, tokens[token_index - 1]) )
        {
            token_index = (int16_t)(token_index - 1);
            if ( token_index <= 0 )
                goto done_lookup;
        }
        matched = 1;
    }
done_lookup:

    if ( (unsigned char)matched )
    {
        hs_console_mode = 1;
        result = hs_compile_and_evaluate(command);
        hs_console_mode = 0;
        return result;
    }

    console_printf_only_if_open(0, "Requested function \"%s\" cannot be executed now.", short_name);
    return matched;
}
