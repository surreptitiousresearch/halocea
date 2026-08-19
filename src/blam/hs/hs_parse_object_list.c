/* hs_parse_object_list @0x83776E20 — parse an object-list atom. Reparses as object-name type 43 (the generic
 * object designator), then restores the script type to 23 (object_list).
 *
 * Deviation: sets u1 (the constant-type slot at offset 2) to 43 then runs hs_parse_object_name. */

#include "headers/hs_syntax_node.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int hs_parse_object_name(int expression_index);

int hs_parse_object_list(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    node->___u1.constant_type = hs_type_object_name;
    node->type = hs_type_object_name;
    int result = hs_parse_object_name(expression_index);
    node->type = hs_type_object_list;
    return result;
}
