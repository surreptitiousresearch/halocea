#include "CONSOLE_CALLBACK.h"

// con::CONSOLE_CALLBACK::CONSOLE_CALLBACK() @ 0x827EA400
// Default constructor: wires the vtable, points pageName at the process-wide shared
// empty-string singleton (lazy-inits it on first use, per dsTSTRING<char>::UnsafeInitEmpty),
// and clears the visibility/version-valid flags.
con::CONSOLE_CALLBACK::CONSOLE_CALLBACK()
{
    // __vftable is set implicitly by the compiler-generated constructor prologue
    // (decompile shows an explicit store to &CONSOLE_CALLBACK::`vftable' here).
    this->pageName.pBuffer = nullptr;
    this->pageName.UnsafeInitEmpty();
    this->isVisible = 0;
    this->pageVersionValid = 0;
}
