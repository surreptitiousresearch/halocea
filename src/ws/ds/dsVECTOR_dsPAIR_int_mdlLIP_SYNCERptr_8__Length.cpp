#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::Length @ 0x823CF420
template<>
int dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::Length() const
{
    return this->nElem;
}
