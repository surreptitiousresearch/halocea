/* network_game_data_packet @ 0x841766E8 -- third-order .data (data_closure.py);
   reconstructed from binary bytes. */
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"

extern data_packet_field network_game_data_packet_fields[];

data_packet_definition network_game_data_packet =
{
    .name             = "network_game_data_packet",
    .flags            = 0,
    .size             = 1360, /* +0x08 high 16 bits (0x0550) */
    .version          = 1,    /* +0x08 low 16 bits */
    .fields           = network_game_data_packet_fields,
    .initialized_flag = 0,
};
