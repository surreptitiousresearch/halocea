#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"

// Translation-unit-local helper from the original aiGLOSS.cpp anonymous namespace
// (`anonymous namespace'::FindInVector, 0x8323A680). Character-wise search for `s` inside the
// disabled-strikes vector; returns its index or -1. Given internal linkage (matching the DB's
// anonymous-namespace symbol), each consuming TU gets its own copy.
namespace {

inline int FindInVector(const dsVECTOR<dsTSTRING<char>, 8> *v, const dsTSTRING<char> *s)
{
    int nElem = v->nElem;
    for ( int i = 0; i < nElem; ++i )
    {
        const dsTSTRING<char> *elem = &(*v)[i];
        dsTSTRING_BUF_HEADER<char> *sBuf = s->pBuffer;
        dsTSTRING_BUF_HEADER<char> *elemBuf = elem->pBuffer;
        bool match;
        if ( elemBuf == sBuf )
        {
            match = true; // same interned buffer
        }
        else
        {
            int elemLen = elemBuf->strLen;
            if ( elemLen != sBuf->strLen )
            {
                match = false;
            }
            else if ( elemLen == 0 )
            {
                match = true;
            }
            else
            {
                const char *sStr = sBuf->str;
                const char *elemStr = elemBuf->str;
                const char *elemEnd = &elemBuf->str[elemLen];
                int diff = 0;
                do
                {
                    diff = (unsigned char)*elemStr - (unsigned char)*sStr;
                    if ( diff != 0 )
                        break;
                    ++elemStr;
                    ++sStr;
                }
                while ( elemStr != elemEnd );
                match = ( diff == 0 );
            }
        }
        if ( match )
            return i;
    }
    return -1;
}

} // anonymous namespace
