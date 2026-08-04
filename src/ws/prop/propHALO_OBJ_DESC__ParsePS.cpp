#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ps/psARRAY.h"

extern "C" int strcmp(const char *lhs, const char *rhs); // boundary — CRT

// propHALO_OBJ_DESC::ParsePS(psSECTION, const dsTSTRING<char>&) @ 0x823EBBF8
// After the propBASE_DESC base parse, read "timeTerm" and the "use" string array, OR-ing a
// distinct bit into objUse.state per recognised name.
//
// CAVEAT: the disassembly implements this as ~19 hand-inlined byte-by-byte strcmp loops against
// a fixed set of constant strings (the decompiler rendered each as a separate do-while over
// HIDWORD/pointer-walk locals with reg-alloc noise). Verified every constant/bit pairing against
// the disassembly's `this->objUse.state | <bit>` stores. The very first comparison
// ("DEVICE_POWER_STATE") short-circuits (`break`s the scan loop) straight to the same
// `objUse.state | 0x1` fallback that the final "no match" path also reaches -- i.e. it is
// functionally identical to just letting DEVICE_POWER_STATE fall through the else-chain below,
// so it is reproduced here as an ordinary chain entry rather than a separate early-exit, with no
// change in behaviour. `ps` is accessed through psSECTION_INTERFACE (a same-layout,
// methods-only accessor over the raw psSECTION handle, per its header) via reference downcast.
void propHALO_OBJ_DESC::ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr)
{
    propBASE_DESC::ParsePS(ps, hintErr);

    psSECTION_INTERFACE &psi = static_cast<psSECTION_INTERFACE &>(ps);

    psi.GetFloat("timeTerm", &this->timeTerm, 0);

    this->objUse.state = 0;

    psARRAY useArray = psi.GetArray("use", 0);
    int useCount = useArray.Length();
    for (int i = 0; i < useCount; ++i)
    {
        // dsTSTRING<char> has no reversed default constructor of its own; the "empty def"
        // argument below is built the same way sslERROR::sslERROR() builds its own empty desc
        // string (pBuffer=nullptr then UnsafeInitEmpty), matching this loop's own inlined
        // guarded-singleton init in the disassembly.
        dsTSTRING<char> emptyDef;
        emptyDef.pBuffer = nullptr;
        emptyDef.UnsafeInitEmpty();

        dsTSTRING<char> useName = useArray.GetStr(i, emptyDef);
        const char *use = useName.CStr();

        unsigned int flag;
        if (!strcmp(use, "DEVICE_POWER_STATE"))
            flag = 0x1;
        else if (!strcmp(use, "ACT_CAMO"))
            flag = 0x2;
        else if (!strcmp(use, "SHIELD"))
            flag = 0x4;
        else if (!strcmp(use, "DEVICE_LOCKED"))
            flag = 0x8;
        else if (!strcmp(use, "DEVICE_POSITION_TRANSP"))
            flag = 0x10;
        else if (!strcmp(use, "FUNC"))
            flag = 0x1E0; // FUNC_A|FUNC_B|FUNC_C|FUNC_D combined
        else if (!strcmp(use, "FUNC_A"))
            flag = 0x20;
        else if (!strcmp(use, "FUNC_B"))
            flag = 0x40;
        else if (!strcmp(use, "FUNC_C"))
            flag = 0x80;
        else if (!strcmp(use, "FUNC_D"))
            flag = 0x100;
        else if (!strcmp(use, "SHIELD_COLOR"))
            flag = 0x200;
        else if (!strcmp(use, "TRACK_IN_A"))
            flag = 0x400;
        else if (!strcmp(use, "TRACK_IN_B"))
            flag = 0x800;
        else if (!strcmp(use, "TRACK_IN_C"))
            flag = 0x1000;
        else if (!strcmp(use, "TRACK_IN_D"))
            flag = 0x2000;
        else if (!strcmp(use, "TRACK_OUT_A"))
            flag = 0x4000;
        else if (!strcmp(use, "TRACK_OUT_B"))
            flag = 0x8000;
        else if (!strcmp(use, "TRACK_OUT_C"))
            flag = 0x10000;
        else if (!strcmp(use, "TRACK_OUT_D"))
            flag = 0x20000;
        else
            flag = 0x1;

        this->objUse.state |= flag;
    }
}
