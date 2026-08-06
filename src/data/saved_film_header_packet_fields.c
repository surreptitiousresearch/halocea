/* saved_film_header_packet_fields @ 0x84176698 (.data, 40 bytes)
 * DB applied_types: data_packet_field saved_film_header_packet_fields[4];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 type                       = 0x0003
 *        +0x02 count                      = 0x0001
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 *   [ 1] +0x00 type                       = 0x0002
 *        +0x02 count                      = 0x0003
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 *   [ 2] +0x00 type                       = 0x0000
 *        +0x02 count                      = 0x0002
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 *   [ 3] +0x00 type                       = 0x0009
 *        +0x02 count                      = 0x0000
 *        +0x04 first_version              = 0x0000
 *        +0x06 last_version               = 0x0000
 *        +0x08 size                       = 0x0000
 * third-order .data (data_closure.py);
 *    reconstructed from binary bytes.
 */
#include "headers/data_packet_field.h"
#include "headers/packet_field_type.h"

data_packet_field saved_film_header_packet_fields[] =
{
    { .type = __pack_long,  .count = 1, .first_version = 0, .last_version = 0, .size = 0 },
    { .type = __pack_short, .count = 3, .first_version = 0, .last_version = 0, .size = 0 },
    { .type = __pack_pad,   .count = 2, .first_version = 0, .last_version = 0, .size = 0 },
    { .type = __pack_end,   .count = 0, .first_version = 0, .last_version = 0, .size = 0 },
};
