#pragma once

struct dsDATA; // ws-engine ds — forward (per-stream user context, passed through)
namespace ds { struct BIT_STREAM; } // ws-engine ds bit stream — forward

namespace ds_data {

// Per-T network (BIT_STREAM) pack/unpack policy. For a T that supplies its own member
// Pack/Unpack the statics just forward to them (see the reversed specializations, e.g.
// dsDATA_TYPE_PACK<haloRAW_INPUT_X360_ENV>::Pack/Unpack, DB 0x823BD5A8 / 0x823BD5E0).
template<class T>
struct dsDATA_TYPE_PACK {
    static void Pack(const T &data, ds::BIT_STREAM &stream, const dsDATA &userData);
    static void Unpack(ds::BIT_STREAM &stream, T &data, const dsDATA &userData);
};

} // namespace ds_data
