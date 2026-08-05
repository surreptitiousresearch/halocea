#pragma once
#include <stdint.h>
/* saved_film_frame_header_packet — film frame header packet template; size is the body byte count that
 * follows. Rebound to the DB type saved_film_frame_header. */

typedef struct saved_film_frame_header
{
    int16_t size; /* 0x0 */
    int16_t pad;  /* 0x2 */
} saved_film_frame_header;

typedef saved_film_frame_header saved_film_frame_header_packet_t;
