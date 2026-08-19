/* hs_parse @0x83778778 — type-check and parse one HaloScript expression node against `expected_type`. Already
 * typed nodes (type != 0) succeed immediately. Primitive nodes (flag 1) record the expected type in both the
 * type and constant-type slots and run the primitive parser; group nodes run the non-primitive parser. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse_primitive(int expression_index);
extern uint8_t hs_parse_nonprimitive(int expression_index);

uint8_t hs_parse(int expression_index, int16_t expected_type)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    if ( node->type )
        return 1;
    node->type = expected_type;
    if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) == 0 )
        return hs_parse_nonprimitive(expression_index);
    node->___u1.constant_type = expected_type;
    return hs_parse_primitive(expression_index);
}
