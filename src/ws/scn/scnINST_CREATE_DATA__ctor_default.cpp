// scnINST_CREATE_DATA::scnINST_CREATE_DATA() @0x823B1598 — default-construct: state=0,
// nameInst/nameTpl/nameClass each adopt the process-wide shared empty-string singleton
// (dsTSTRING<char>::UnsafeInitEmpty, ref-bumped), then default-construct affixes/ps.
#include "../../headers/ws/scn/scnINST_CREATE_DATA.h"

scnINST_CREATE_DATA::scnINST_CREATE_DATA()
    : state(0), nameInst(), nameTpl(), nameClass(), matrInst(), affixes(), ps()
{
    // The dsTSTRING<char> default ctor (adopts the shared empty-string singleton, ref-bumped) is
    // what the disassembly repeats three times inline (UnsafeInitEmpty's lazy-init guard); the
    // member-initializer-list default ctors above are that same operation.
}
