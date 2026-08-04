/* message_server_game_update_packet @ 0x84176780 -- third-order .data (data_closure.py);
   reconstructed from binary bytes. */
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"

extern data_packet_field message_server_game_update_packet_fields[];

data_packet_definition message_server_game_update_packet =
{
    .name             = "message_server_game_update_packet",
    .flags            = 0,
    .size             = 1044, /* +0x08 high 16 bits (0x0414) */
    .version          = 1,    /* +0x08 low 16 bits */
    .fields           = message_server_game_update_packet_fields,
    .initialized_flag = 0,
};
