/* saved_film_group @ 0x841767B4 (.data, 20 bytes)
 * DB applied_types: data_packet_group_definition saved_film_group;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82113924 -> "saved_film_group"
 *   +0x04 packet_type_count          = 0x0004
 *   +0x06 packet_class_count         = 0x0001
 *   +0x08 maximum_decoded_packet_size = 0x00000800
 *   +0x0C maximum_encoded_packet_size = 0x00000800
 *   +0x10 packets                    = 0x84176794 -> saved_film_group_packets
 */
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
