#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"
#include "../../headers/ws/ssl/sslERROR.h" // sslERROR / SSL_ERR_NO_ERROR

// propHALO_OBJ_DESC::InitSslClass(const dsTSTRING<char>&) @ 0x823EB808
// After the propBASE_DESC base init (bail out on failure), and only once (guarded by
// isEntSslClassInited), unshare this class's sslClass ref and register its eight OnChange event
// declarations (In/Out x A-D) on it, short-circuiting on the first registration error.
//
// CAVEAT: the disassembly shows each `sslClass.RegisterEvent(...)` result assigned into a running
// sslERROR local and its temporary immediately released (a refcount-decrement-then-conditional-
// dlFree pair inlined right after each `sslERROR::operator=`). That inlined pair is exactly the
// compiler-generated destructor call for RegisterEvent's returned-by-value sslERROR temporary
// (same convention as sslSOURCE_REF::~sslSOURCE_REF / sslERROR::sslERROR) -- reproduced here as
// plain RAII (`err = ...RegisterEvent(...)`), which the compiler lowers to the identical
// assign-then-destroy-temporary sequence seen in the disassembly. Also, register order confirms
// `this->sslClass.RegisterEvent(...)`: r4 (the "result" slot per the DB's nominal argument
// naming) is actually &this->sslClass across every call in this function, while r3 (nominally
// "this") is the sret-returned sslERROR's stack slot -- i.e. this call, unlike TypeID, really is
// this-and-result register-swapped relative to the DB's naming, verified against the
// UnshareSslClass call immediately before it (r3=this directly, r4=0, r5=0).
//
// The errHint argument passed to every RegisterEvent call is a fixed empty-string byte
// (empty_string, a single 0x00 in rodata), not the incoming `hintErr` parameter -- modeled here
// as a shared empty C string literal.
int propHALO_OBJ_DESC::InitSslClass(const dsTSTRING<char> &hintErr)
{
    static const char *const kEmptyRegisterEventErrHint = ""; // empty_string

    if (!propBASE_DESC::InitSslClass(hintErr))
        return 0;

    if (this->isEntSslClassInited.val)
        return 1;

    this->UnshareSslClass(false, false);

    sslERROR err;

    err = this->sslClass.RegisterEvent("OnChangeInA(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeInA);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeInB(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeInB);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeInC(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeInC);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeInD(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeInD);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeOutA(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeOutA);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeOutB(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeOutB);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeOutC(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeOutC);
    if (err.id)
        return 0;

    err = this->sslClass.RegisterEvent("OnChangeOutD(vnew : float, vold : float)", 0,
                                        kEmptyRegisterEventErrHint, &this->sslEventId_OnChangeOutD);

    return err.id == SSL_ERR_NO_ERROR;
}
