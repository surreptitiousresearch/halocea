#include "../headers/hcex/haloSTORAGE_TASK.h"

// 0x823F4B50 — haloSTORAGE_TASK::`vector deleting destructor'(char a2). Standard MSVC
// compiler-synthesized thunk: runs ~haloSTORAGE_TASK() (already reversed in
// haloSTORAGE_TASK__dtor.cpp), then conditionally calls operator delete(this) when bit 0 of `a2`
// is set. No separate hand-written C++ source exists for it.
