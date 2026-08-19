/* hs_compile_and_evaluate @0x837303D8 — compile a single HaloScript console expression and evaluate it.
 * Lines containing ';' or consisting only of whitespace are treated as comments/blank and skipped. A bare
 * command is auto-wrapped: "(cmd args)" for a non-global, "(set global value)" for a writable global with an
 * argument, or left as-is for a parenthesized expression or a no-arg global read. On a compile error the
 * (single-line) error message is surfaced; on success the compiled expression is run. Afterwards, if a
 * recompile was requested, the whole HaloScript source is rebuilt and the runtime re-initialized for the map.
 * Returns 1 if an expression was evaluated, 0 otherwise.
 *
 * Deviations:
 *   - The inlined ctype-table whitespace test (__pctype_func()[...] & 8) is reproduced with isspace().
 *   - The ';'-found test `v21[0] &= (found != 0) - 1` (zeroes the line when a ';' is present) is reproduced
 *     explicitly.
 *   - hs_compile_source/hs_node_gc/hs_compile_dispose and the recompile-path dispose/init calls are void
 *     no-arg; the decompiler chained them through r3. Unchained. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/data_array.h"
#include <string.h>
#include <ctype.h>
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int sprintf_0(char *string, const char *format, ...);
extern void hs_compile_initialize(uint8_t permanent);
extern int16_t hs_find_global_by_name(const char *name);
/* extern corrected to match def (hs_compile_expression.c): returns int (expression index, or -1). */
extern int hs_compile_expression(int source_size, const char *source_data, const char **error_message_pointer, const char **error_source_pointer);
extern int hs_runtime_evaluate(int expression_index);
extern void hs_compile_dispose(void);
extern uint8_t hs_rebuild_source(void);
extern uint8_t hs_compile_source(void);
extern void hs_node_gc(void);
extern void data_make_invalid(data_array *data);
extern void data_dispose(data_array *data);
extern void hs_runtime_dispose_from_old_map(void);
extern void object_lists_dispose_from_old_map(void);
extern void hs_allocate(void);
extern uint8_t hs_scenario_postprocess(uint8_t hack);
extern void object_lists_initialize_for_new_map(void);
extern void hs_runtime_initialize_for_new_map(void);

int hs_compile_and_evaluate(const char *expression)
{
    const char *to_compile = expression;
    int evaluated = 0;

    char line[1024];
    strncpy(line, expression, 0x400u);
    line[1023] = 0;

    /* a ';' anywhere makes the line a comment: blank out the first char */
    if ( strchr(line, ';') )
        line[0] = 0;

    char is_blank = 1;
    if ( line[0] )
    {
        char *p = line;
        while ( isspace((unsigned char)*p) )
        {
            if ( !*++p )
                goto check_blank;
        }
        is_blank = 0;
    }

check_blank:
    if ( !is_blank )
    {
        int16_t wrap_mode = 0;
        hs_compile_initialize(0);
        if ( line[0] != '(' )
        {
            char *space = strchr(line, ' ');
            if ( space )
                *space = 0;

            if ( hs_find_global_by_name(line) == -1 )
            {
                wrap_mode = 1;          /* not a global: "(cmd args)" */
            }
            else if ( space )
            {
                wrap_mode = 2;          /* writable global with arg: "(set global value)" */
            }

            if ( space )
                *space = ' ';
        }

        char wrapped[1088];
        if ( wrap_mode == 1 )
        {
            sprintf_0(wrapped, "(%s)", line);
            to_compile = wrapped;
        }
        else if ( wrap_mode == 2 )
        {
            sprintf_0(wrapped, "(set %s)", line);
            to_compile = wrapped;
        }

        const char *error_text = 0;
        char *error_message = 0;
        int compiled = hs_compile_expression(strlen(to_compile), to_compile,
                                             &error_text, (const char **)&error_message);
        if ( compiled == -1 )
        {
            if ( error_text && error_message )
            {
                char *newline = strchr(error_message, '\n');
                if ( newline )
                    *newline = 0;
            }
        }
        else
        {
            evaluated = 1;
            hs_runtime_evaluate(compiled);
        }
        hs_compile_dispose();
    }

    if ( recompile )
    {
        if ( hs_rebuild_source() )
        {
            hs_compile_source();
            if ( hs_syntax_data )
            {
                hs_node_gc();
                if ( hs_syntax_data_allocated )
                {
                    data_make_invalid(hs_syntax_data);
                    data_dispose(hs_syntax_data);
                    hs_syntax_data_allocated = 0;
                }
                hs_syntax_data = 0;
            }
            hs_runtime_dispose_from_old_map();
            object_lists_dispose_from_old_map();
            scenario *active_scenario = global_scenario_index == -1 ? 0 : global_scenario;
            hs_allocate();
            if ( active_scenario && active_scenario->hs_syntax_data.size )
                hs_scenario_postprocess(0);
            object_lists_initialize_for_new_map();
            hs_runtime_initialize_for_new_map();
        }
        recompile = 0;
    }

    return evaluated;
}
