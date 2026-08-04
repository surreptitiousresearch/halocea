/* haloEngineCtrlInit @0x823CCD88 — lazily allocate and construct the global haloENGINE_CONTROL
 * (idempotent: returns true immediately if one already exists), then run its Init(). */

#include "../headers/hcex/haloENGINE_CONTROL.h"

extern haloENGINE_CONTROL *haloEngineCtrl;

extern void *operator new(size_t size, const char *file, int line);
extern void  haloENGINE_CONTROL_ctor(haloENGINE_CONTROL *self); /* haloENGINE_CONTROL::haloENGINE_CONTROL @0x823CAF50 */
extern int   haloENGINE_CONTROL_Init(haloENGINE_CONTROL *self); /* haloENGINE_CONTROL::Init @0x823CCAE8 */

int haloEngineCtrlInit(void)
{
    haloENGINE_CONTROL *self;

    if (haloEngineCtrl)
        return 1;

    self = (haloENGINE_CONTROL *)operator new(0x1Cu, "D:\\Projects\\code\\HCEX\\sources\\halo_engine_ctrl.cpp", 0x472);
    if (self)
        haloENGINE_CONTROL_ctor(self);
    else
        self = 0;

    haloEngineCtrl = self;
    return haloENGINE_CONTROL_Init(self);
}
