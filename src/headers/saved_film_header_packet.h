#pragma once
/* saved_film_header_packet — leading 'film' marker packet written at the start of a recording.
 * The payload IS the DB type saved_film_header (canonical definition in saved_film_header.h). The
 * DB has NO type of this name; the symbol saved_film_header_packet @0x841766C0 is the generic
 * data_packet_definition descriptor (blam_data_globals.h). The alias therefore carries the `_t`
 * suffix, matching the sibling saved_film_frame_header_packet_t. */

#include "saved_film_header.h"

typedef saved_film_header saved_film_header_packet_t;
