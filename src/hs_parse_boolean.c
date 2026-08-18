/* hs_parse_boolean @0x83776080 — parse a boolean atom. The token text (compiled_source[source_offset]) is
 * matched case-sensitively against the accepted spellings: "false"/"off"/"0" => 0, "true"/"on"/"1" => 1.
 * Anything else is an error. The parsed value is written to the node's data field (low byte).
 *
 * Deviation: the decompiler inlines each spelling test as a byte-compare loop; reproduced with strcmp. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int strcmp(const char *a, const char *b);

int hs_parse_boolean(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    const char *token = &hs_compile_globals.compiled_source[node->source_offset];

    if ( !strcmp(token, "false") || !strcmp(token, "off") || !strcmp(token, "0") )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store `stb r10, 0x10(r7)` @0x83776224
         * (was an *(unsigned char*)&node->data pun); hs_evaluate passes node->data whole-word to hs_cast and
         * boolean consumers extract value >> 24 2026-08-18 */
        node->data = (unsigned int)((uint8_t)0) << 24;
        return 1;
    }
    if ( !strcmp(token, "true") || !strcmp(token, "on") || !strcmp(token, "1") )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store `stb r11, 0x10(r7)` @0x83776230
         * (was an *(unsigned char*)&node->data pun); boolean consumers extract value >> 24 2026-08-18 */
        node->data = (unsigned int)((uint8_t)1) << 24;
        return 1;
    }
    hs_compile_globals.__noop = "i expected \"true\" or \"false\".";
    hs_compile_globals.error_offset = node->source_offset;
    return 0;
}
