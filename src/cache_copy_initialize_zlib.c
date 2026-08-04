/* cache_copy_initialize_zlib @0x83808008 — resets the zlib decompression stream and starts an inflate pass. */

#include "headers/simple_decompressor_definition.h"

extern int inflateInit_(z_stream *strm, const char *version, int stream_size);

void cache_copy_initialize_zlib(simple_decompressor_definition *self)
{
    self->zlib_stream.next_in = 0;
    self->zlib_stream.avail_in = 0;
    self->zlib_stream.next_out = 0;
    self->zlib_stream.avail_out = 0;
    inflateInit_(&self->zlib_stream, "1.1.3", 56);
}
