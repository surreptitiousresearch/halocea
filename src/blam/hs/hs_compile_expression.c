/* hs_compile_expression @0x83779C50 — compile a single HaloScript expression from `source_data` into the
 * syntax tree, wrapping it in a synthetic 2-node "(begin <expr>)"-style group so hs_parse can type-check it
 * against the expected type (4). Returns the root expression index, or -1 on error (with *error_message_pointer
 * / *error_source_pointer set). When no scenario is loaded the source is heap-allocated; otherwise it is laid
 * down in the scenario's string-constant tag data. */

#include <stdint.h>
#include <string.h>
#include "headers/scenario.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tokenizer.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void skip_whitespace(char **c);
extern int hs_tokenize(hs_tokenizer *state);
extern int datum_new(data_array *data);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_compile_expression(
        int source_size,
        const char *source_data,
        const char **error_message_pointer,
        const char **error_source_pointer)
{
    char *source_buffer;
    int base_offset;
    hs_tokenizer tokenizer[24];

    if ( source_size >= 1024 )
        return -1;

    if ( global_scenario_index == -1 )
    {
        base_offset = 0;
        source_buffer = dlMalloc(source_size + 1, "D:\\Projects\\code\\HCEX\\sources\\hs\\hs_compile.c", 0xB0u);
        hs_compile_globals.malloced = 1;
    }
    else
    {
        source_buffer = (char *)global_scenario->hs_string_constants.address;
        base_offset = global_scenario->hs_string_constants.size - 1024;
    }

    hs_compile_globals.compiled_source = source_buffer;
    memcpy(&source_buffer[base_offset], source_data, source_size);
    hs_compile_globals.compiled_source_size = base_offset + source_size;
    hs_compile_globals.compiled_source[base_offset + source_size] = 0;
    hs_compile_globals.__noop = nullptr;

    *error_message_pointer = nullptr;
    *error_source_pointer = nullptr;
    tokenizer[0].cursor = &hs_compile_globals.compiled_source[base_offset];
    hs_compile_globals.error_offset = -1;
    skip_whitespace(&tokenizer[0].cursor);
    if ( !*tokenizer[0].cursor )
        return -1;

    int expression_index = hs_tokenize(tokenizer);
    const char *error_message = hs_compile_globals.__noop;
    if ( !hs_compile_globals.__noop )
    {
        int wrapper_index = datum_new(hs_syntax_data);
        int begin_index = datum_new(hs_syntax_data);
        if ( wrapper_index != -1 && begin_index != -1 )
        {
            hs_syntax_node *wrapper = &HS_SYNTAX_NODE(wrapper_index);
            hs_syntax_node *begin = &HS_SYNTAX_NODE(begin_index);
            wrapper->data = begin_index;
            wrapper->next_node_index = -1;
            wrapper->source_offset = HS_SYNTAX_NODE(expression_index).source_offset;
            wrapper->flags = 0;
            begin->next_node_index = expression_index;
            begin->source_offset = -1;
            begin->___u1.function_index = _hs_function_inspect;   /* console eval prints via inspect */
            begin->flags = (1u << _hs_syntax_node_primitive_bit);
            begin->type = hs_function_name;
            if ( hs_parse(wrapper_index, hs_type_void) )
                return wrapper_index;
        }
        error_message = hs_compile_globals.__noop;
    }

    *error_message_pointer = error_message;
    int error_offset = hs_compile_globals.error_offset;
    if ( hs_compile_globals.error_offset != -1 )
    {
        hs_compile_globals.error_offset -= base_offset;
        *error_source_pointer = &source_data[error_offset - base_offset];
    }
    return -1;
}
