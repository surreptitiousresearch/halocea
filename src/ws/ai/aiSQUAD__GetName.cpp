#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiSQUAD::GetName() const @ 0x832A9600
// ?GetName@aiSQUAD@@UBA?AV?$dsTSTRING@D@@XZ
//
// Return the squad's instance name (the owned animINST's `name`) as a fresh ref-counted string,
// substituting the empty string when the entity has no instance name. Returned by value (sret).
dsTSTRING<char> aiSQUAD::GetName() const
{
    const char *name = pInst->name; // entENTITY::pInst @0x15C -> animINST::name @0x28
    if (!name)
        name = ""; // byte_8200155A

    // The C-string ctor's body is exactly `pBuffer = 0; UnsafeInit(name, -1, 0)` — matching the
    // binary's construct-in-place into the sret buffer.
    return dsTSTRING<char>(name);
}
