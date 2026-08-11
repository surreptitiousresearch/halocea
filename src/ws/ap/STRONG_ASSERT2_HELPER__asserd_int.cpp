// STRONG_ASSERT2_HELPER::asserd<int> @0x823B17C0 — strong-assert with one named integer operand.
// Renders "<name> = <value>\n" for the failing operand, appends it to the (empty) shared context
// string, and drives the terminal STRONG_ASSERT_DUMMY::Crash handler with the built message.
// This is the T=int instantiation of the STRONG_ASSERT2_HELPER::asserd<T> template.
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include <string.h>

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // 0x825210C0
namespace ds { dsTSTRING<char> ToString(int value); }   // int -> decimal string

// DEVIATION: this TU declared `namespace STRONG_ASSERT_DUMMY { void Crash(void*, ...); }` — a
// free function that mangles to ?Crash@STRONG_ASSERT_DUMMY@@YAX...@Z, a symbol the image does not
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // contain. The one real Crash is the member.

struct STRONG_ASSERT2_HELPER {
    template<class T>
    static void asserd(const char *cond, const char *file, int line, const char *name, const T &value);
};

template<>
void STRONG_ASSERT2_HELPER::asserd<int>(const char *cond, const char *file, int line,
                                        const char *name, const int &value)
{
    // Lazily-initialised shared empty string (dsTSTRING<char>::UnsafeInitEmpty's emptyStr static);
    // a copy of it (sharing the buffer, refcount-bumped) is the accumulator for the message.
    static dsTSTRING<char> emptyStr;
    static bool emptyInited = false;
    if (!emptyInited)
    {
        emptyInited = true;
        emptyStr.pBuffer = nullptr;
        emptyStr.pBuffer = emptyStr.AllocBuffer(0, 0);
    }

    dsTSTRING<char> message;
    message.pBuffer = emptyStr.pBuffer;
    ++message.pBuffer->refCount;

    dsTSTRING<char> valueStr = ds::ToString(value);
    dsTSTRING<char> line1 = dsSPrintf("%s = %s\n", name, valueStr.pBuffer->str);

    // append line1's characters onto message (reserve then raw copy, as the shipped code does).
    int at = message.pBuffer->strLen;
    message.Insert(at, line1.pBuffer->strLen);
    memcpy(&message.pBuffer->str[at], line1.pBuffer->str, line1.pBuffer->strLen);

    static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(cond, file, line, message.pBuffer->str);
    // (message / line1 / valueStr release their shared buffers via dsTSTRING's destructor)
}
