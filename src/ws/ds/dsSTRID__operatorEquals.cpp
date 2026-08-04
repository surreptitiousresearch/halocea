#include "dsSTRID.h"

// dsSTRID::operator==(const char*) const @ 0x82528118
// Character-wise (strcmp-style) equality against a NUL-terminated literal: walks both strings
// byte-by-byte until a mismatch or a NUL in `this->id`.
bool dsSTRID::operator==(const char *rhs) const
{
    const char *lhs = this->id;
    for (;;) {
        unsigned char lhsChar = (unsigned char)*lhs;
        unsigned char rhsChar = (unsigned char)*rhs;
        if (lhsChar == 0)
            return rhsChar == 0;
        if (lhsChar != rhsChar)
            return false;
        ++lhs;
        ++rhs;
    }
}
