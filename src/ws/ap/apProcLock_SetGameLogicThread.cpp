// apProcLock_SetGameLogicThread @ 0x82506CE8 — ap_proc_lock.cpp
// Publish `threadId` as the game-logic processor and rebind the per-class processor-affinity
// guards (dsTSTRING<char>/<wchar_t>, psSYSTEM, psSECTION) that assert single-threaded access to
// that same processor. Each of those is a static apPROCESSOR_LOCK member on its owning class.
#include "../../headers/ws/ap/apPROCESSOR_LOCK.h"

extern int gameLogicThread; // current game-logic processor id

// boundary — static apPROCESSOR_LOCK members: dsTSTRING<char>::processorLock,
// dsTSTRING<wchar_t>::processorLock, psSYSTEM::processorLock, psSECTION::processorLock.
extern apPROCESSOR_LOCK dsTSTRING_char_processorLock;
extern apPROCESSOR_LOCK dsTSTRING_wchar_processorLock;
extern apPROCESSOR_LOCK psSYSTEM_processorLock;
extern apPROCESSOR_LOCK psSECTION_processorLock;

void apProcLock_SetGameLogicThread(int threadId)
{
    gameLogicThread = threadId;
    dsTSTRING_char_processorLock.processor_  = threadId;
    dsTSTRING_wchar_processorLock.processor_ = threadId;
    psSYSTEM_processorLock.processor_        = threadId;
    psSECTION_processorLock.processor_       = threadId;
}
