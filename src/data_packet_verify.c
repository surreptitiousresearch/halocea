/* data_packet_verify @ 0x83820098 — verify a packet definition once.
 * Forwards to the 4-arg field walker _data_packet_verify @0x8381F8E8 (the real linker symbol carries a
 * leading underscore, so there is no clash with this function's own name). */

#include <stdint.h>
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"
/* DEVIATION: the decompiler modeled {initialized_flag@0, fields@4}; the DB-verified layout has
 * initialized_flag@0x10 and fields@0xC. Reusing the canonical header corrects the offsets. */

extern void _data_packet_verify(const data_packet_definition *packet_definition, int16_t *byte_count_reference,
                                data_packet_field *first_field, int16_t *field_count_reference);

void data_packet_verify(data_packet_definition *packet_definition)
{
    int16_t field_count;
    int16_t scratch[7];

    if ( !packet_definition->initialized_flag )
    {
        _data_packet_verify(packet_definition, scratch, packet_definition->fields, &field_count);
        packet_definition->initialized_flag = 1;
    }
}
