#pragma once
// ws-engine ds: a bit-granularity network serialization stream. Used throughout the HCEX
// network-pack layer (haloPLAYER_ACTION_PACKER::Pack/Unpack, haloRAW_INPUT_X360_ENV::Pack/Unpack,
// haloPLAYER_ACTIONS_ENV::Pack/Unpack). BOUNDARY -- only the fields/methods those callers touch
// are modeled; RawWrite/RawRead/ByteReorder bodies are external to this batch.
// DB-verified layout (types_members ds::BIT_STREAM): __vftable@0, m_pData@4 (unsigned __int8 *),
// m_size@8, m_pos@12, m_isWrite@16 (bool), m_isOwnBuffer@17 (bool), m_isOverflow@18 (bool),
// m_isInitialized@19 (bool), m_reordering@20 (bool) -- size 24 (padded).

namespace ds {

struct BIT_STREAM_vtbl; // boundary -- vtable shape not needed by these callers

typedef struct BIT_STREAM {
    BIT_STREAM_vtbl   *__vftable;      // 0x00
    unsigned __int8   *m_pData;        // 0x04
    unsigned int       m_size;         // 0x08
    unsigned int       m_pos;          // 0x0C
    bool               m_isWrite;      // 0x10
    bool               m_isOwnBuffer;  // 0x11
    bool               m_isOverflow;   // 0x12
    bool               m_isInitialized;// 0x13
    bool               m_reordering;   // 0x14

    // ?RawWrite@BIT_STREAM@ds@@QAAXPBXII@Z @ 0x82612018 -- write `sizeBits` bits of `data` at
    // `offsetBits` into the stream (advances m_pos). Not reversed here; boundary.
    void RawWrite(const void *data, unsigned int sizeBits, unsigned int offsetBits);

    // ?RawRead@BIT_STREAM@ds@@QAAXPAXII@Z @ 0x826121D0 -- read `sizeBits` bits from the stream
    // into `data` at `dstOffset`. Not reversed here; boundary.
    void RawRead(void *data, unsigned int sizeBits, unsigned int dstOffset);
} BIT_STREAM;

// ?ByteReorder@ds@@YAXPAEHH@Z @ 0x82521C70 -- swap byte order of `data` (network <-> host),
// used when m_reordering is set. Not reversed here; boundary.
void ByteReorder(unsigned __int8 *data, int size, int reorderGain);

} // namespace ds
