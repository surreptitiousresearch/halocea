#include <string.h>
#include "sslCLASS_REF.h"
#include "sslERROR.h"

/* HCEX boundary flat-bridge for sslCLASS_REF::AddCbFunc @ 0x82524DD0 (member-callback overload,
 * re-sourced in sslCLASS_REF__AddCbFunc_member.cpp). The HCEX bridge TUs (hcex_cine_init.cpp via
 * hcex_cine_init_boundary.h) call it through this flattened C-linkage-style spelling, with the
 * hidden sret result pointer explicit as `outErr` and `this` as `self` — exactly the register
 * layout the binary call site uses (r3 = result, r4 = this). `cbFunc` arrives as a plain code
 * address (void *); the member overload takes it as an sslCB_HOST pointer-to-member, so the bits
 * are transplanted via memcpy (same slot in the 360 ABI; a direct cast is ill-formed C++).
 * `hostObj` is the errHint text (the shared empty-string sentinel at every HCEX call site). */
void sslCLASS_REF_AddCbFunc(sslERROR *outErr, sslCLASS_REF *self, const char *sig, void *cbFunc,
                            const void *hostObj, int *outExtra)
{
    sslCB_MEMBER_FN memberFn = 0;
    memcpy(&memberFn, &cbFunc, sizeof cbFunc);
    *outErr = self->AddCbFunc(sig, memberFn, (const char *)hostObj, outExtra);
}
