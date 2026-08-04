/* saved_film_group_packets @ 0x84176794 -- second-order .data; data_packet_group_packet[4] reconstructed from binary bytes. */
#include "../headers/data_packet_group_packet.h"

extern data_packet_definition saved_film_header_packet;
extern data_packet_definition network_game_data_packet;
extern data_packet_definition saved_film_frame_header_packet;
extern data_packet_definition message_server_game_update_packet;

data_packet_group_packet saved_film_group_packets[4] = {
    [0] = { .packet_class = 0, .definition = &saved_film_header_packet },
    [1] = { .packet_class = 0, .definition = &network_game_data_packet },
    [2] = { .packet_class = 0, .definition = &saved_film_frame_header_packet },
    [3] = { .packet_class = 0, .definition = &message_server_game_update_packet },
};
