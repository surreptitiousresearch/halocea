#pragma once
/* z_stream — standard zlib decompression stream state (56 bytes, DB-confirmed layout matches upstream zlib). */

/* DB-verified (types_members internal_state): the binary's PDB models zlib's
 * private state as a 4-byte shell `{ int dummy; }` — accessed only by pointer. */
typedef struct internal_state { int dummy; } internal_state;

typedef struct z_stream
{
    unsigned char *next_in;    /* 0x00 */
    unsigned int   avail_in;   /* 0x04 */
    unsigned int   total_in;   /* 0x08 */
    unsigned char *next_out;   /* 0x0C */
    unsigned int   avail_out;  /* 0x10 */
    unsigned int   total_out;  /* 0x14 */
    char          *msg;        /* 0x18 */
    internal_state *state;     /* 0x1C */
    void          *(*zalloc)(void *opaque, unsigned int items, unsigned int size); /* 0x20 */
    void           (*zfree)(void *opaque, void *address);                          /* 0x24 */
    void          *opaque;     /* 0x28 */
    int            data_type;  /* 0x2C */
    unsigned int   adler;      /* 0x30 */
    unsigned int   reserved;   /* 0x34 */
} z_stream;                    /* 56 bytes */

/* DB carries both spellings (z_stream and z_stream_s, identical 56-byte layout, upstream zlib tag). */
typedef z_stream z_stream_s;
