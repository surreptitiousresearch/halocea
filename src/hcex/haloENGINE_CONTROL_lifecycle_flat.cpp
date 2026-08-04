#include "../headers/hcex/haloENGINE_CONTROL.h"
#include "../headers/ws/ds/ds_new.h" // placement new

// Authored def TUs for the flattened C-flattening spellings the hcex bridge callers declare
// extern (haloEngineCtrlInit/Term, hcex_get_*, verify_checkpoint, haloInit_boundary.h); each
// forwards to the reversed method in src/hcex/engine_control/.

// haloENGINE_CONTROL::haloENGINE_CONTROL @ 0x823CAF50
void haloENGINE_CONTROL_ctor(haloENGINE_CONTROL *self)
{
    new (self) haloENGINE_CONTROL();
}

// haloENGINE_CONTROL::~haloENGINE_CONTROL @ 0x823CAC60
void haloENGINE_CONTROL_dtor(haloENGINE_CONTROL *self)
{
    self->~haloENGINE_CONTROL();
}

// haloENGINE_CONTROL::Init @ 0x823CCAE8
int haloENGINE_CONTROL_Init(haloENGINE_CONTROL *self)
{
    return self->Init();
}

// haloENGINE_CONTROL::InitSettings @ 0x823CC698
void haloENGINE_CONTROL_InitSettings(haloENGINE_CONTROL *self)
{
    self->InitSettings();
}

// haloENGINE_CONTROL::IsCoop @ 0x823C1338
int haloENGINE_CONTROL_IsCoop(haloENGINE_CONTROL *self)
{
    return self->IsCoop();
}
