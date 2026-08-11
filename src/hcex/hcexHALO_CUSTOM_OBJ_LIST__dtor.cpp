// hcexHALO_CUSTOM_OBJ_LIST::~hcexHALO_CUSTOM_OBJ_LIST() @ 0x823D97D8
//
// Tear down cdList (its dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8> destructor runs the
// per-element teardown + frees the backing store), then re-stamp __vftable back to the
// instMANAGER_HANDLER base slot (defensive re-stamp before the base dtor runs, matching the disasm).
// The cdList member destructor is emitted implicitly by the compiler; no manual body is written for
// it here to avoid double-releasing.

#include "../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

hcexHALO_CUSTOM_OBJ_LIST::~hcexHALO_CUSTOM_OBJ_LIST()
{
    // implicit: cdList.~dsVECTOR()
    this->__vftable = &instMANAGER_HANDLER::vftable; // reset to base slot
}
