#include "psITERATOR.h"
#include "psSECTION_KEY_REF.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../apCL.h"
// 0x8251B600 (??0psITERATOR@@QAA@XZ) — default ctor: an empty, unfiltered iterator positioned
// before any records. The name filter adopts the shared empty-string singleton and the record
// vector is built with the ps_section.cpp allocation call-site cookie.

psITERATOR::psITERATOR()
    : name()                                                     // shared empty-string singleton
    , records(apCL{ "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 1248 })
{
    this->type  = PS_TYPEID_INVALID;
    this->flags = 0;
    this->pos   = 0;
}
