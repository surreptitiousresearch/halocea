#pragma once
/* bitstream_seek_origin — origin for bitstream_seek's absolute/relative bit-position offset. */
typedef enum bitstream_seek_origin
{
    _bitstream_seek_beginning = 0,
    _bitstream_seek_current   = 1,
    _bitstream_seek_end       = 2,
} bitstream_seek_origin;
