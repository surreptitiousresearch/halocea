/* first_object_type_definition @ 0x8477A3B0 (.data, 4 bytes)
 * DB applied_types: object_type_definition *first_object_type_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * first_object_type_definition — head of object-type-definition list (data-bss); DB pointer(4), ledger size 16 (next-gap overcount)
 */
#include "../headers/object_type_definition.h"

object_type_definition *first_object_type_definition;
