/* haloEngineCtrlTerm @0x823CBFF0 — destroy the global haloENGINE_CONTROL created by
 * haloEngineCtrlInit, if one exists. */

#include "../headers/hcex/haloENGINE_CONTROL.h"

extern haloENGINE_CONTROL *haloEngineCtrl;

extern void haloENGINE_CONTROL_dtor(haloENGINE_CONTROL *self); /* haloENGINE_CONTROL::~haloENGINE_CONTROL @0x823CAC60 */
extern void operator_delete(void *block);                      /* operator delete */

void haloEngineCtrlTerm(void)
{
    if (haloEngineCtrl)
    {
        haloENGINE_CONTROL *self = haloEngineCtrl;
        haloENGINE_CONTROL_dtor(self);
        operator_delete(self);
        haloEngineCtrl = 0;
    }
}
