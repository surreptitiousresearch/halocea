/* hs_compile @0x83779E58 — compile a whole HaloScript source file: append its text to the growing compiled
 * source buffer, then tokenize and parse each top-level expression (primitive or non-primitive) in turn. On
 * the first parse failure it records the error message/offset and stops. */

#include <stdint.h>
#include <string.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tokenizer.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern void skip_whitespace(char **c);
extern int hs_tokenize(hs_tokenizer *state);
extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);

void hs_compile(
        int source_file_size,
        const char *source_file_data,
        const char **error_message_pointer,
        const char **error_source_pointer)
{
    char *appended = nullptr;
    hs_tokenizer tokenizer[24];

    char *grown = dlRealloc(
                      hs_compile_globals.compiled_source,
                      hs_compile_globals.compiled_source_size + source_file_size + 1,
                      "D:\\Projects\\code\\HCEX\\sources\\hs\\hs_compile.c",
                      0xFEu);
    if ( grown )
    {
        hs_compile_globals.compiled_source = grown;
        appended = &grown[hs_compile_globals.compiled_source_size];
        memcpy(appended, source_file_data, source_file_size);
        hs_compile_globals.compiled_source_size += source_file_size;
        hs_compile_globals.compiled_source[hs_compile_globals.compiled_source_size] = 0;
    }

    tokenizer[0].cursor = appended;
    if ( !appended )
    {
        *error_message_pointer = "couldn't allocate memory for compiled source.";
        return;
    }

    hs_compile_globals.__noop = nullptr;
    *error_message_pointer = nullptr;
    *error_source_pointer = nullptr;
    hs_compile_globals.error_offset = -1;

    unsigned char ok = 1;
    skip_whitespace(&tokenizer[0].cursor);
    do
    {
        if ( !*tokenizer[0].cursor )
            break;
        int expression_index = hs_tokenize(tokenizer);
        skip_whitespace(&tokenizer[0].cursor);
        ok = hs_compile_globals.__noop == nullptr;
        if ( !hs_compile_globals.__noop )
        {
            ok = 1;
            hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
            if ( !node->type )
            {
                node->type = hs_special_form;
                if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
                {
                    node->___u1.function_index = 1;
                    ok = hs_parse_primitive(expression_index);
                }
                else
                {
                    ok = hs_parse_nonprimitive(expression_index);
                }
            }
        }
    }
    while ( ok );

    if ( !ok )
    {
        *error_message_pointer = hs_compile_globals.__noop;
        hs_compile_globals.error_since_initialize = 1;
        if ( hs_compile_globals.error_offset != -1 )
        {
            hs_compile_globals.error_offset = hs_compile_globals.error_offset
                                            - hs_compile_globals.compiled_source_size
                                            + source_file_size;
            *error_source_pointer = &source_file_data[hs_compile_globals.error_offset];
        }
    }
}
