// FUNCTION_INDEX entry: hkReferenceCountError @0x838D7798 (?hkReferenceCountError@@YAXPBVhkReferencedObject@@PBD@Z)
#include "../headers/havok/hkError.h"

/* Build the verbose reference-count diagnostic for an object and route it to the
   global hkError sink; trap if the handler asks for a break. */
void hkReferenceCountError(const hkReferencedObject *object, const char *why)
{
    hkErrStream stream;
    char buffer[544];
    hkOstream *os;
    int refCount;

    hkErrStream_construct(&stream, buffer, 512);
    refCount = object->m_referenceCount;
    os = hkOstream_insertString((hkOstream *)&stream, "Reference count error on object ");
    os = hkOstream_insertObject(os, object);
    os = hkOstream_insertString(os, " with ref count of ");
    os = hkOstream_insertInt(os, refCount);
    os = hkOstream_insertString(os, " in ");
    os = hkOstream_insertString(os, why);
    os = hkOstream_insertString(os, ".\n");
    os = hkOstream_insertString(os, " * Are you calling delete instead of removeReference?\n");
    os = hkOstream_insertString(os, " * Have you called removeReference too many times?\n");
    os = hkOstream_insertString(os, " * In a multithreaded environment, what is the hkReferencedObject lock mode you use (see setLockMode())?\n");
    os = hkOstream_insertString(os, " * Is this a valid object?\n");
    hkOstream_insertString(os, " * Do you have more than 32768 references? (unlikely)\n");
    if (hkError_message(hkError_s_instance, MESSAGE_ERROR, 744944344, buffer,
                        ".\\System\\Error\\hkError.cpp", 31)) /* backslashes escaped 2026-07-30 */
    {
        __trap();
    }
    hkOstream_destruct((hkOstream *)&stream);
}
