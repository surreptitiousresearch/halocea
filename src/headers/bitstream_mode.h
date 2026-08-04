#pragma once
/* bitstream_mode — read vs write. _bitstream_mode_write == 0 (observed in disassembly). */
typedef enum bitstream_mode
{
    _bitstream_mode_write = 0,
    _bitstream_mode_read  = 1,
} bitstream_mode;
