/* saved_film_frame_header_packet @ 0x8417671C -- third-order .data (data_closure.py);
   reconstructed from binary bytes. */
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
