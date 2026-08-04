#pragma once
// Generic "value with a defaulted/zero-initialized constructor" wrapper the corpus's decompiler
// output names this way at several call sites; modeled generically since every DB instantiation
// seen so far is a plain single-field passthrough (e.g. DEFAULT_CTOR<bool> size 1,
// DEFAULT_CTOR<usePROCESSOR *> size 4).
template<class T>
struct DEFAULT_CTOR {
    T val; // 0x00
};
