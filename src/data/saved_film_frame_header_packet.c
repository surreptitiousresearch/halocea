/* saved_film_frame_header_packet @ 0x8417671C (.data, 20 bytes)
 * DB applied_types: data_packet_definition saved_film_frame_header_packet;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211395C -> "saved_film_frame_header_packet"
 *   +0x04 flags                      = 0x00000000
 *   +0x08 size                       = 0x0004
 *   +0x0A version                    = 0x0001
 *   +0x0C fields                     = 0x841766FC -> saved_film_frame_header_packet_fields
 *   +0x10 initialized_flag           = 0x00
 * third-order .data (data_closure.py);
 *    reconstructed from binary bytes.
 */
#include "headers/data_packet_definition.h"
#include "headers/data_packet_field.h"

extern data_packet_field saved_film_frame_header_packet_fields[];

data_packet_definition saved_film_frame_header_packet =
{
    .name             = "saved_film_frame_header_packet",
    .flags            = 0,
    .size             = 4,    /* +0x08 high 16 bits */
    .version          = 1,    /* +0x08 low 16 bits */
    .fields           = saved_film_frame_header_packet_fields,
    .initialized_flag = 0,
};
