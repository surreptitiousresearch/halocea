// apProcLock_GetGameLogicThread @ 0x82506CD8 — ap_proc_lock.cpp
// Return the processor/thread id currently designated as the game-logic thread.
extern int gameLogicThread; // 0x8413F... file-scope current game-logic processor id

int apProcLock_GetGameLogicThread()
{
    return gameLogicThread;
}
