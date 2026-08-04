#pragma once
// Scaleform GFx smart pointer (third-party GFx SDK). 4 bytes — a single owning object pointer.
template<class T>
struct GPtr {
    T *pObject; /* 0x00 */
};
