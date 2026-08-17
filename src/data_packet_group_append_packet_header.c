/* data_packet_group_append_packet_header @0x837DF910 */
#include <stdint.h>
#include "headers/data_packet_group_definition.h"
#include "headers/blam_data_globals.h"


BOOL data_packet_group_append_packet_header(data_packet_group_definition *group_definition, uint8_t *encoded_packet, int16_t *encoded_packet_size, int16_t packet_type)
{
    int size = *encoded_packet_size;

    if ( (unsigned int)(size + 1) >= group_definition->maximum_encoded_packet_size )
    {
        global_data_packet_groups_error_string = "couldn't append header to encoded packet";
        return 0;
    }

    encoded_packet[size] = packet_type;
    ++*encoded_packet_size;
    global_data_packet_groups_error_string = 0;
    return 1;
}
