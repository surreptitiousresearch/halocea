// apMEM_DUMPER_BASE::Dump @ 0x82C4AB30
// Re-entrancy-guarded report shim: gather the report via coreDump, emit it via Write.
#include "apMEM_DUMPER_BASE.h"

void apMEM_DUMPER_BASE::Dump(const char *fileName, unsigned int typesMask)
{
    if (isDumpInProgress)
        return;

    apMEM_DUMPER_BASE_vtbl *vtbl = this->__vftable;
    isDumpInProgress = true;
    // Dispatch the virtual coreDump with its DB-declared (self, fileName, typesMask) signature.
    // The disasm (82C4AB68) sets neither r4 nor r5, relying on the pass-through of Dump's own
    // incoming args; forwarding them explicitly reproduces that register state without a cast.
    vtbl->coreDump(this, fileName, typesMask);
    this->__vftable->Write(this, fileName, nullptr);
    isDumpInProgress = false;
}
