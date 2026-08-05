/* hs_syntax_nth @0x8368C900 — walk `n` links down the next_node_index chain of a HaloScript syntax
 * expression and return the resulting expression index (n<=0 returns the starting index unchanged). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_syntax_node.h"
#include "headers/blam_data_globals.h"

int hs_syntax_nth(int expression_index, int16_t n)
{
    int16_t remaining = n;
    if ( n > 0 )
    {
        do
        {
            remaining = (int16_t)(remaining - 1);
            expression_index = HS_SYNTAX_NODE(expression_index).next_node_index;
        }
        while ( remaining > 0 );
    }
    return expression_index;
}
