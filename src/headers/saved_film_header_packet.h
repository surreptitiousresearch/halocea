#pragma once
/* saved_film_header_packet — leading 'film' marker packet written at the start of a recording.
 * The payload IS the DB type saved_film_header (canonical definition in saved_film_header.h); the
 * same-named global symbol is the generic data_packet_definition descriptor that points at it. */

#include "saved_film_header.h"

typedef saved_film_header saved_film_header_packet;
