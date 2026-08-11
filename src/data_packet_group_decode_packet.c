/* data_packet_group_decode_packet @0x837DF810 — decode one packet from a packet group. The last byte of
 * the encoded buffer is the packet type index; it is validated against the group, its class must match the
 * expected class, then the type's definition decodes the payload (header byte stripped) into decoded_packet.
 * On success packet_type is filled and TRUE is returned; on any failure an error string is recorded and
 * FALSE returned. */

#include <stdint.h>
#include "headers/data_packet_group_definition.h"
#include "headers/blam_data_globals.h"

extern uint8_t data_packet_decode(data_packet_definition *packet_definition, void *encoded_packet, int16_t encoded_packet_size, void *decoded_packet, int16_t *version_reference, int16_t *actual_encoded_packet_size_reference);

uint8_t data_packet_group_decode_packet(
    data_packet_group_definition *group_definition,
    void *decoded_packet,
    char *encoded_packet,
    int16_t *encoded_packet_size,
    int16_t *packet_type,
    int16_t *packet_version,
    int16_t expected_packet_class)
{
    const char *error = 0;
    if ( *encoded_packet_size )
    {
        char *end = &encoded_packet[*encoded_packet_size];
        int type_index = *(end - 1);
        if ( type_index < 0 || type_index >= group_definition->packet_type_count )
        {
            error = "got packet with bad type";
        }
        else
        {
            data_packet_group_packet *packet = &group_definition->packets[type_index];
            if ( packet->packet_class == expected_packet_class )
            {
                int16_t payload_size = *encoded_packet_size - 1;
                *encoded_packet_size = payload_size;
                data_packet_definition *definition = packet->definition;
                if ( !definition || data_packet_decode(definition, encoded_packet, payload_size, decoded_packet, packet_version, 0) )
                    *packet_type = *(end - 1);
                else
                    error = "got packet which wouldn't decode";
            }
            else
            {
                error = "got packet with mismatched class";
            }
        }
    }
    else
    {
        error = "got packet with no header";
    }
    global_data_packet_groups_error_string = error;
    return error == 0;
}
