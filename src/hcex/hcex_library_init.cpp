/* hcex_library_init @0x823D7280 — register the two HCEX SSL script callbacks that toggle the
 * bridge library's visibility ("HcexHideLibrary()" / "HcexShowLibrary()") on the SSL global
 * object's class. Both registrations release their temporary sslOBJ_REF/sslCLASS_REF handles
 * (and, in the decompiler's spilled temporary, an intermediate ref-counted buffer) immediately
 * after the AddCbFunc call. */

#include "../headers/ws/ssl/sslSYSTEM.h"
#include "../headers/ws/ssl/sslCLASS_REF.h"
#include "../headers/ws/ssl/sslOBJ_REF.h"

extern sslSYSTEM *gsSslSystem;
extern void       cbHcexHideLibrary(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
extern void       cbHcexShowLibrary(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
extern const char dsStrongAssertMessage[]; /* empty_string — empty string, used as the doc/help arg */

void hcex_library_init(void)
{
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("HcexHideLibrary()", cbHcexHideLibrary, dsStrongAssertMessage, 0);
    }
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("HcexShowLibrary()", cbHcexShowLibrary, dsStrongAssertMessage, 0);
    }
}
