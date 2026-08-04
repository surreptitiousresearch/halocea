#pragma once
// net::CLIENT_ID — relay client identifier. DB-verified (types_enum_values net::CLIENT_ID); the
// only named enumerator is the sentinel. Stored as a 4-byte int.
namespace net {
enum CLIENT_ID {
    UNKNOWN_CLIENTID = -1,
};
} // namespace net
