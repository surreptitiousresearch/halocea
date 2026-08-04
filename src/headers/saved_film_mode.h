#pragma once
/* Saved-film record/playback constants (save_film.c).
 * DB anon enum _2233F9610690B787A7EC2EFF8E8FFB70 — film mode. */
enum saved_film_mode
{
    _saved_film_inactive_mode  = 0,
    _saved_film_recording_mode = 1,
    _saved_film_playback_mode  = 2,
    NUMBER_OF_SAVED_FILM_MODES  = 3,
};

/* DB anon enum _07BC6CD4E22FA1FC1BC35F92FDBA128F — packet types/classes + header magics. */
enum saved_film_packet
{
    SAVED_FILM_GAME_CODE                    = 1651269997, /* 'blam' */
    SAVED_FILM_VERSION                      = 1,
    _saved_film_packet_class                = 0,
    NUMBER_OF_SAVED_FILM_PACKET_CLASSES     = 1,
    _saved_film_header_packet               = 0,
    _network_game_data_packet               = 1,
    _saved_film_frame_header_packet         = 2,
    _message_server_game_update_packet      = 3,
    NUMBER_OF_SAVED_FILM_PACKET_TYPES       = 4,
    MAXIMUM_DECODED_SAVED_FILM_PACKET_SIZE  = 2048,
};

/* DB anon enum _5CAB69BE156703F384BD2E7954EDC224 */
enum saved_film_io { SAVED_FILM_IO_BUFFER_SIZE = 16384 };
