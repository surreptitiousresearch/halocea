/* hs_parse_object @0x83776C80 — parse an object atom. The literal "none" stores -1 (no object). Otherwise the
 * object designator types are reparsed as the corresponding object-name types: the script type is temporarily
 * raised by 6 (mapping an object type to its object-name designator) and hs_parse_object_name does the work,
 * after which the type is restored.
 *
 * Deviation: the "none" compare is inlined as a byte loop in the decompiler; reproduced with strcmp. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

/* Shift between an object designator type and its object-name counterpart
 * (first_hs_object_name_type - first_hs_object_type == number_of_hs_object_types). */
#define HS_OBJECT_TO_OBJECT_NAME_SHIFT (first_hs_object_name_type - first_hs_object_type)

extern int strcmp(const char *a, const char *b);
extern int hs_parse_object_name(int expression_index);

int hs_parse_object(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);

    if ( !strcmp(&hs_compile_globals.compiled_source[node->source_offset], "none") )
    {
        node->data = (unsigned int)-1;
        return 1;
    }

    node->type = (int16_t)(node->type + HS_OBJECT_TO_OBJECT_NAME_SHIFT);
    node->___u1.constant_type = node->type;
    int result = hs_parse_object_name(expression_index);
    node->type = (int16_t)(node->type - HS_OBJECT_TO_OBJECT_NAME_SHIFT);
    return result;
}
