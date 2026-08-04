/* data_packet_group_encode_packet @0x837DF990 — encode one packet from a group and append its 1-byte type
 * header. Records a human-readable error string on failure and returns whether encoding succeeded. */

#include <stdint.h>
#include "headers/data_packet_group_definition.h"
#include "headers/blam_data_globals.h"


#include "headers/data_packet_definition.h"
extern uint8_t data_packet_encode(data_packet_definition *packet_definition, int16_t version, void *packet, void *buffer, int16_t *buffer_size, int16_t maximum_buffer_size);

uint8_t data_packet_group_encode_packet(data_packet_group_definition *group_definition, void *decoded_packet,
                                    unsigned char *encoded_packet, int16_t *encoded_packet_size,
                                    int16_t packet_type, int16_t packet_version)
{
    const char *error = 0;
    if ( data_packet_encode(group_definition->packets[packet_type].definition, packet_version, decoded_packet,
                            encoded_packet, encoded_packet_size, group_definition->maximum_encoded_packet_size) )
    {
        int size = *encoded_packet_size;
        if ( (unsigned int)(size + 1) >= (unsigned int)group_definition->maximum_encoded_packet_size )
        {
            error = "couldn't append header to encoded packet";
        }
        else
        {
            encoded_packet[size] = (char)packet_type;
            ++*encoded_packet_size;
        }
    }
    else
    {
        error = "couldn't encode packet";
    }
    global_data_packet_groups_error_string = error;
    return error == 0;
}
