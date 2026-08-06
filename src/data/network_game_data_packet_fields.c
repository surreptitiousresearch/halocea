/* network_game_data_packet_fields @ 0x841766D4 (.data, 20 bytes)
 * DB applied_types: data_packet_field network_game_data_packet_fields[2];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 type                       = 0x0008
 *        +0x02 count                      = 0x0550
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 *   [ 1] +0x00 type                       = 0x0009
 *        +0x02 count                      = 0x0000
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 * third-order .data (data_closure.py);
 *    reconstructed from binary bytes.
 */
#include "headers/data_packet_field.h"
#include "headers/packet_field_type.h"

data_packet_field network_game_data_packet_fields[] =
{
    { .type = __pack_fixed_data, .count = 1360, .first_version = 0, .last_version = 0, .size = 0 },
    { .type = __pack_end,        .count = 0,    .first_version = 0, .last_version = 0, .size = 0 },
};
