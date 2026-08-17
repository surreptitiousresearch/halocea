/* ?GetArrayObject@psSECTION_PARSER@@IAAHAAVdsDATA@@@Z @0x82A2E928 */
#include "psSECTION_PARSER.h"
#include "psARRAY.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
// 0x82A2E944 — parse a `[ v, v, ... ]` array body: read the comma-separated value list via
// GetValuesArr(), require the closing `]` token, then box the values into a fresh psARRAY and store
// that as `obj`.

int psSECTION_PARSER::GetArrayObject(dsDATA &obj)
{
    dsVECTOR<dsDATA, 8> values;

    psTOKEN closeTok;
    if (!GetValuesArr(values, 1) || !GetToken(closeTok, 3))
        return 0;

    if (closeTok.id != ']') {
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("closing ']' not found", -1, 0);
        errorDesc = msg;
        return 0;
    }

    psARRAY array;
    array.SetElements(values);

    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<psARRAY>(array);
    obj.StoreValue(boxed);
    if (boxed.type)
        boxed.type->Destroy(&boxed.storage);
    return 1;
}
