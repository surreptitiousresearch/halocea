#include "iaDESC.h"

// iaDESC::RegisterSmlState(const dsTSTRING<char>&) — 0x826AEE58
// Resolve `nameState` to its numeric sml state id for actors described by this iaDESC: forwards
// straight to sml::STATE_DESC::Register on the embedded smlDesc table (offset 0x88).
int iaDESC::RegisterSmlState(const dsTSTRING<char> &nameState)
{
    return smlDesc.Register(nameState);
}
