/* data_packet_group_initialize @ 0x837DF778 — verify every packet definition in a group.
 * DEVIATION: the decompiler modeled an inline `packets[]` array of {definition} with packet_type_count@0;
 * the DB-verified layout (data_packet_group_definition.h) has packet_type_count@4 and `packets` as a
 * pointer@0x10 to 8-byte records (definition@4). Field names match, so reusing the canonical header
 * corrects the offsets and indexing. */

#include "headers/data_packet_group_definition.h"

extern void data_packet_verify(data_packet_definition *packet_definition);

void data_packet_group_initialize(data_packet_group_definition *group)
{
    int i;
    for ( i = 0; i < group->packet_type_count; ++i )
    {
        data_packet_definition *def = group->packets[i].definition;
        if ( def )
            data_packet_verify(def);
    }
}
