#pragma once
// ws-engine tracked allocation operators. The engine routes heap allocation through a
// custom global operator new carrying the source file/line of the allocation site
// (D:\Projects\code\common\src.sys ... allocation tracking). Reconstructed call sites
// preserve the original (size, file, line) tuple.
//
// x64 note: the first formal parameter of any operator new MUST be size_t (C2821). On the
// original Xbox 360 PPC build size_t was 32-bit (unsigned int); here it is whatever the
// target defines, so declare with size_t to stay correct on both. DEVIATION: PPC used
// `unsigned int`; size_t is the portable spelling and is identical on the 32-bit target.
#include <cstddef>

// Engine tracked global operator new (size, source file, source line). boundary — engine heap.
extern void *operator new(size_t size, const char *file, int line);
extern void *operator new[](size_t size, const char *file, int line);

// Standard placement new (construct at an already-allocated address).
inline void *operator new(size_t, void *place) noexcept { return place; }
