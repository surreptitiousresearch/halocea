#pragma once
// ws-engine ds: lightweight wrapper returned by ds::MAP::Begin(). Its sole member `iter` is the
// map's starting iterator; MAP_ITERATOR's ctor(const BEGIN_HELPER&) just copies it out. Existing
// only so Begin()'s sret return type is distinct from MAP_ITERATOR itself (matches ds_map.hpp's
// begin()/BEGIN_HELPER idiom). DB-verified layout: iter@0 (single member, same size as T).

namespace ds { namespace impl {

template<class T>
struct BEGIN_HELPER {
    T iter; // 0x00

    BEGIN_HELPER() {}
    // 0x82681098 (BEGIN_HELPER<MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,
    // dsNODE_CACHE_ALLOCATOR>>) — copy ctor, bytewise copy of the wrapped iterator.
    BEGIN_HELPER(const BEGIN_HELPER &other) : iter(other.iter) {}
};

} } // namespace ds::impl
