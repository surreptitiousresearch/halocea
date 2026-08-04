#include "../headers/data_packet_group_definition.h"
#include "../headers/data_packet_group_packet.h"
/* name -> "saved_film_group"; packets -> saved_film_group_packets */
extern data_packet_group_packet saved_film_group_packets[];
data_packet_group_definition saved_film_group = {
    "saved_film_group",       /* name */
    4,                        /* packet_type_count */
    1,                        /* packet_class_count */
    0x800,                    /* maximum_decoded_packet_size = 2048 */
    0x800,                    /* maximum_encoded_packet_size = 2048 */
    saved_film_group_packets, /* packets */
};
