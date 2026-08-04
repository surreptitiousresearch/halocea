#include "../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

// 0x823D95B0
// True when the instance's class name is exactly "halo_no_model": inline strcmp against the literal.
bool hcexHALO_CUSTOM_OBJ_LIST::IsSkipCreation(scnINST_CREATE_DATA *pInstCD)
{
    const char *pLiteral = "halo_no_model";
    const char *pName = pInstCD->nameClass.pBuffer->str;
    int diff;
    do
    {
        int ch = (unsigned char)*pName;
        diff = ch - (unsigned char)*pLiteral;
        if ( ch == 0 )
            break;
        ++pName;
        ++pLiteral;
    }
    while ( !diff );
    return diff == 0;
}
