#pragma once
/* saved_film_frame_header_packet — film frame header packet template; size is the body byte count that
 * follows. Rebound to the DB type saved_film_frame_header. */

typedef struct saved_film_frame_header
{
    __int16 size; /* 0x0 */
    __int16 pad;  /* 0x2 */
} saved_film_frame_header;

typedef saved_film_frame_header saved_film_frame_header_packet_t;
