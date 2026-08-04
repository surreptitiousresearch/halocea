#include "psSECTION_PARSER.h"
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// 0x82A2FE4C — parse a `{ ... }` section body: allocate a fresh empty section, hand a referenced
// copy of it to GetSection() to fill in, then require the closing `}` token. On success, boxes the
// filled section as a dsDATA into `obj`.

int psSECTION_PARSER::GetSectionObject(dsDATA &obj)
{
    psSECTION section;
    static_cast<psSECTION_INTERFACE &>(section).CreateEmpty();

    // GetSection() takes its psSECTION parameter by value (a referenced copy it owns and releases
    // internally), matching normal C++ pass-by-value semantics for `section` here.
    if (!GetSection(section))
        return 0;

    psTOKEN closeTok;
    if (!GetToken(closeTok, 3))
        return 0;

    if (closeTok.id != '}') {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("closing '}' not found", -1, 0);
        errorDesc = msg;
        return 0;
    }

    dsDATA boxed = static_cast<psSECTION_INTERFACE &>(section).CreateObject();
    obj.StoreValue(boxed);
    if (boxed.type)
        boxed.type->Destroy(&boxed.storage);
    return 1;
}
