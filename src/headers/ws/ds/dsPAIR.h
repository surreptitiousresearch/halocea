#pragma once
// ws-engine ds: a simple key/value pair (the value type of many sorted dsVECTORs).
// DB-verified layout (types_members dsPAIR<dsTSTRING<char>,int>): key@0, val@sizeof(K).
// Template params: K = key type; V = value type.

template<class K, class V>
struct dsPAIR {
    K key; // 0x00
    V val; // naturally aligned after key

    dsPAIR() {}
    // Construct from a key/value pair (copy both). Used by dsVECTOR<dsPAIR<...>>::InsertSorted call
    // sites that build a fresh entry to insert (e.g. cbAddCine -> listCine).
    dsPAIR(const K &k, const V &v) : key(k), val(v) {}

    // Copy-construct / copy-assign / destroy. Declared (not inline-defaulted) because some
    // instantiations are emitted out of line with the members' non-trivial special members inlined
    // into the pair's own — e.g. dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>> at
    // 0x823B8080 / 0x823B8980 / 0x823B3ED8 — and are reversed per-instantiation.
    dsPAIR(const dsPAIR<K, V> &that);
    dsPAIR<K, V> &operator=(const dsPAIR<K, V> &that);
    ~dsPAIR();
};
