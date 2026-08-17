/* ?Pack@haloPLAYER_ACTIONS_ENV@@QBAXAAVBIT_STREAM@ds@@ABVdsDATA@@@Z @0x823BC7D8 */
#include <stdint.h>
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/ws/ds/dsDATA.h"

// 0x823BC7FC — serialize u0, then u1, then skip_cine as a raw 32-bit field (byte-reordered when
// the stream is in reordering mode).
void haloPLAYER_ACTIONS_ENV::Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const
{
    u0.Pack(stream, userCtx);
    u1.Pack(stream, userCtx);

    unsigned int skipCineField = skip_cine;
    if ( stream.m_reordering )
        ds::ByteReorder(reinterpret_cast<uint8_t *>(&skipCineField), 4, 4);
    stream.RawWrite(&skipCineField, 0x20u, 0);
}
