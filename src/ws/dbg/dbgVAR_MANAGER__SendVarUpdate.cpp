#include "dbgVAR_MANAGER.h"
#include "../ps/psARRAY.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../con_/MSG_PROTOCOL.h"

/* CAVEAT: the decompiler mistyped the sole parameter as `dsDATA *pVar`; the disasm shows the
   first indirect call dispatches through *r31's vtable slot +4 (dbgVAR_vtbl::GetAsPsData), and the
   mangled name (?SendVarUpdate@dbgVAR_MANAGER@@AAAXPAVdbgVAR@@@Z) confirms the real parameter type
   is `dbgVAR *`. Reconstructed accordingly below.
   The optimizer heavily reuses stack slots for the temporary psARRAY / boxed dsDATA /
   psSECTION_INTERFACE / dsTSTRING<char> objects (one CreateObject call site's `this` argument
   lands on a stack slot the decompiler never shows being initialized), so the exact per-temporary
   storage layout is not fully recoverable from the disasm alone; the sequence below is the
   best-effort reconstruction of the algorithm the decompiler and disasm agree on. The trailing
   refcount-decrement-then-dlFree pattern in the tail of the function (decrement an offset-0 int,
   free at zero) matches dsTSTRING<char>'s buffer-header refcount shape (dsTSTRING_BUF_HEADER::
   refCount@0) rather than a dsDATA_TYPE teardown, so it is modeled here as `messageBody`'s implicit
   destructor at scope exit (not an explicit call, since ReleaseBuffer is protected). */

// con::MSG_PROTOCOL boundary global -- the debug-console connection SendVarUpdate posts to. DB
// symbol is typed `con::CONSOLE_SYS *`, but only the inherited MSG_PROTOCOL surface is used here
// (same convention as apLOG_CON_CB__SendAllLog.cpp's `apConsole`).
extern con::MSG_PROTOCOL *apConsole;

// dbgVAR_MANAGER::SendVarUpdate @ 0x8255E020
// Notify the debug console's "vars" page that `var`'s value changed: box the variable's current
// value into a one-element array under a "vars" key, build that section to text, and send it as a
// dbg_vars_page/send_vars_update console message (cancelling instead if the page's console version
// is stale).
void dbgVAR_MANAGER::SendVarUpdate(dbgVAR *var)
{
    psARRAY varsArray;
    dsDATA  valueSnapshot;
    var->__vftable->GetAsPsData(var, &valueSnapshot);
    varsArray.PushBack(valueSnapshot);
    // Manually destroy the temporary snapshot now that PushBack has copied it into varsArray --
    // dsDATA has no automatic destructor, so this mirrors the disasm's explicit
    // valueSnapshot.type->Destroy(&valueSnapshot.storage) call through dsDATA_TYPE_vtbl slot 0xC.
    if (valueSnapshot.type)
        valueSnapshot.type->Destroy(&valueSnapshot.storage);

    psSECTION_INTERFACE outputSection;
    outputSection.CreateEmpty();

    dsDATA boxedVars = varsArray.CreateObject();
    // AddKey documented as consuming its by-value `val` -- no separate Destroy for boxedVars.
    outputSection.AddKey("vars", boxedVars, 0, -1);

    dsTSTRING<char> messageBody = outputSection.BuildString();

    fioFILE &msg = apConsole->StartMsg(this->pageName.pBuffer->str, "dbg_vars_page", "send_vars_update");
    msg.Write(messageBody);

    if (this->pageVersionValid)
        apConsole->SendMsg(msg);
    else
        apConsole->CancelMsg(msg);

    // messageBody, outputSection, and varsArray release their buffers/handles here via their
    // (boundary) destructors at scope exit, matching the disasm's trailing refcount-release,
    // ~psSECTION, and ~psARRAY calls.
}
