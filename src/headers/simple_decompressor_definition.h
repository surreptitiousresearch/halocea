#pragma once
/* simple_decompressor_definition @ (cache_copy.c) — async map-copy/decompress worker state. Full layout
 * per DB types_members(simple_decompressor_definition), all 41 members named including the zlib stream,
 * OVERLAPPED[11] array, and read/write request arrays — no opaque padding remains. */

#include "cache_file_header.h"
#include "cache_copy_read_request.h"
#include "cache_copy_write_request.h"
#include "_OVERLAPPED.h"
#include "z_stream.h"

typedef struct simple_decompressor_definition
{
    char                   src_name[261];          /* 0x000 */
    unsigned char          _pad105[3];             /* 0x105 */
    cache_file_header      header;                 /* 0x108 */
    volatile unsigned int  flags;                  /* 0x908 */
    z_stream_s             zlib_stream;            /* 0x90C */
    unsigned char         *zlib_buffer;            /* 0x944 — carved from _pad944 (cache_copy_initialize_and_fill_with_garbage) */
    int                    zlib_buffer_size;       /* 0x948 */
    unsigned char         *next_allocation;        /* 0x94C */
    void                  *copy_start_event;       /* 0x950 */
    void                  *copy_stop_event;        /* 0x954 */
    void                  *copy_complete_event;    /* 0x958 */
    void                  *progress_update_event;  /* 0x95C */
    void                  *copy_thread;            /* 0x960 */
    void                  *allocated_buffer;        /* 0x964 */
    void                  *read_buffers[8];         /* 0x968 */
    void                  *write_buffers[1];        /* 0x988 */
    unsigned char          blocking;               /* 0x98C (2444) */
    unsigned char          pad0[3];                /* 0x98D (2445) */
    void                  *write_file_handle;      /* 0x990 (2448) */
    void                  *read_file_handle;       /* 0x994 (2452) */
    unsigned int           overlapped_in_use_flags[1];    /* 0x998 (2456) — bit N = read/write buffer N in flight */
    unsigned int           overlapped_completed_flags[1]; /* 0x99C (2460) */
    _OVERLAPPED            overlapped[11];         /* 0x9A0 (2464) — one per read/write buffer slot; hEvent
                                                     *  is repurposed (unused by ReadFileEx) to stash the
                                                     *  buffer index for the I/O completion routine */
    cache_copy_read_request  read_requests[8];       /* 0xA7C (2684) */
    cache_copy_write_request write_requests[1];      /* 0xA8C (2700) */
    unsigned char            _padA8E[2];             /* 0xA8E */
    int                    read_file_size;            /* 0xA90 (2704) */
    int                    async_read_bytes_left;     /* 0xA94 (2708) */
    int                    read_bytes_left;           /* 0xA98 (2712) */
    int                    async_write_bytes_left;    /* 0xA9C (2716) */
    int                    write_bytes_left;          /* 0xAA0 (2720) */
    volatile float         read_progress;             /* 0xAA4 (2724) */
    int                    current_write_offset;      /* 0xAA8 (2728) */
    int                    current_read_offset;       /* 0xAAC (2732) */
    cache_copy_read_request *current_request;        /* 0xAB0 (2736) */
    cache_copy_write_request *current_write_request;  /* 0xAB4 (2740) */
    int                    write_requests_pending;    /* 0xAB8 (2744) */
    short                  current_read_sequence_index; /* 0xABC (2748) */
    short                  current_sequence_index;    /* 0xABE (2750) */
    short                  current_write_buffer_index;/* 0xAC0 (2752) */
    short                  current_write_sequence_index; /* 0xAC2 (2754) */
    short                  next_write_sequence_index; /* 0xAC4 (2756) */
    short                  current_read_sequence_count; /* 0xAC6 (2758) */
} simple_decompressor_definition;                    /* 0xAC8 (2760) */

extern simple_decompressor_definition *global_self;

/* decompress_globals @ 0x844A6928 — the singleton decompressor state block (data-bss, 2760 bytes). */
extern simple_decompressor_definition decompress_globals;
