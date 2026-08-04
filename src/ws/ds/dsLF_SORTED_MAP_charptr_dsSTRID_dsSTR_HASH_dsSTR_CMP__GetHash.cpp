#include "dsLF_SORTED_MAP.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

struct dsSTR_HASH; // boundary — string hash policy
struct dsSTR_CMP;  // boundary — string compare policy

// dsSTR_HASH::hash — boundary (mixes the string against the map's two generation words).
extern unsigned int dsSTR_HASH_hash(const char *str, unsigned int a, unsigned int b);

// dsLF_SORTED_MAP<char const*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::GetHash @ 0x82528538
// Hash the key string into a bucket index; assert it stays below ds::BIG_PRIME.
template<>
int dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::GetHash(const char *const *key) const
{
    unsigned int bucketHash = dsSTR_HASH_hash(*key, this->a, this->b);
    if (!IGNORE_STRONG_ASSERT && bucketHash > 0x55555560)
        STRONG_ASSERT_DUMMY().Crash(
            "val >= 0 && val < ds::BIG_PRIME",
            "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_lf_map.h",
            300,
            dsStrongAssertMessage);
    return bucketHash;
}
