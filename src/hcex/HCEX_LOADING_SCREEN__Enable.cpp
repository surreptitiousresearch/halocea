/* HCEX_LOADING_SCREEN::Enable @0x823F3F08 — chain to ui_new::UI_FRAME::Enable, then reset the
 * visible-progress animation state for a fresh loading session. */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"

extern float sHcexVisibleLoadingScreenProgress;
extern float sHcexLoadProgressVelocity;
extern int   sHcexShouldClearPrevFrame;
extern float sHcexTimeAtStartOfLoadingScreenSessionSec;
extern float sHcexLastLoadingScreenUpdateTimeSec;

extern unsigned int GetTickCount();

void HCEX_LOADING_SCREEN::Enable()
{
    ui_new::UI_FRAME::Enable();

    sHcexVisibleLoadingScreenProgress = 0.0f;
    sHcexLoadProgressVelocity = 0.025f;
    sHcexShouldClearPrevFrame = 1;
    sHcexTimeAtStartOfLoadingScreenSessionSec = (float)GetTickCount() * 0.001f;
    sHcexLastLoadingScreenUpdateTimeSec = sHcexTimeAtStartOfLoadingScreenSessionSec;
}
