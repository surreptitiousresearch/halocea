/* network_game_data_packet @ 0x841766E8 (.data, 20 bytes)
 * DB applied_types: data_packet_definition network_game_data_packet;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211397C -> "network_game_data_packet"
 *   +0x04 flags                      = 0x00000000
 *   +0x08 size                       = 0x0550
 *   +0x0A version                    = 0x0001
 *   +0x0C fields                     = 0x841766D4 -> network_game_data_packet_fields
 *   +0x10 initialized_flag           = 0x00
 * third-order .data (data_closure.py);
 *    reconstructed from binary bytes.
 */
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
