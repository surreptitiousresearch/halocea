/* hs_parse_enum @0x83776630 — parse an enum atom. Looks the token up (case-insensitively) in the enum
 * definition for this script type (hs_enum_table[type - 32]); on success the matched ordinal is stored in the
 * node's data field. On failure it builds the canonical "<type> must be "a", "b", or "c"." error message and
 * stores the last ordinal.
 *
 * Deviation: the decompiler inlines every strcat as a find-terminator-then-copy loop; reproduced with strcpy/
 * strcat. The trailing-ordinal stored on the error path is count-1 (the index left in the message-builder
 * loop counter), matching the original. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_enum_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);
extern char *strcat(char *dst, const char *src);
extern int sprintf_0(char *string, const char *format, ...);

int hs_parse_enum(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    const hs_enum_definition *definition = &hs_enum_table[node->type - first_hs_enum_type];
    const char *token = &hs_compile_globals.compiled_source[node->source_offset];

    __int16 ordinal = 0;
    for ( __int16 i = 0; i < definition->count; i = (__int16)(i + 1) )
    {
        if ( !stricmp(token, definition->identifiers[i]) )
            break;
        ordinal = (__int16)(i + 1);
    }

    if ( ordinal != definition->count )
    {
        *(__int16 *)&node->data = ordinal;
        return 1;
    }

    /* not found — build the enumerated-options error message */
    sprintf_0(hs_compile_globals.error_buffer, "%s must be ", hs_type_names[node->type]);
    int count = definition->count;
    __int16 last = 0;
    for ( __int16 i = 0; i < count - 1; i = last )
    {
        strcat(hs_compile_globals.error_buffer, "\"");
        strcat(hs_compile_globals.error_buffer, definition->identifiers[i]);
        strcat(hs_compile_globals.error_buffer, "\", ");
        last = (__int16)(i + 1);
    }
    if ( count > 1 )
        strcat(hs_compile_globals.error_buffer, "or ");
    strcat(hs_compile_globals.error_buffer, "\"");
    strcat(hs_compile_globals.error_buffer, definition->identifiers[last]);
    strcat(hs_compile_globals.error_buffer, "\".");

    hs_compile_globals.__noop = hs_compile_globals.error_buffer;
    hs_compile_globals.error_offset = node->source_offset;
    *(__int16 *)&node->data = last;
    return 0;
}
