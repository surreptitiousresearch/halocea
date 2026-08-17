/* cache_copy_dispose_zlib @0x83808038 */
#include "headers/simple_decompressor_definition.h"

extern int inflateEnd(z_stream *strm);

void cache_copy_dispose_zlib(simple_decompressor_definition *self)
{
    inflateEnd(&self->zlib_stream);
    self->zlib_stream.next_in = 0;
    self->zlib_stream.avail_in = 0;
    self->zlib_stream.next_out = 0;
    self->zlib_stream.avail_out = 0;
}
