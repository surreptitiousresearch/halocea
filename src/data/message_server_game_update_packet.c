/* message_server_game_update_packet @ 0x84176780 (.data, 20 bytes)
 * DB applied_types: data_packet_definition message_server_game_update_packet;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82113938 -> "message_server_game_update_packet"
 *   +0x04 flags                      = 0x00000000
 *   +0x08 size                       = 0x0414
 *   +0x0A version                    = 0x0001
 *   +0x0C fields                     = 0x84176730 -> message_server_game_update_packet_fields
 *   +0x10 initialized_flag           = 0x00
 * third-order .data (data_closure.py);
 *    reconstructed from binary bytes.
 */
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
