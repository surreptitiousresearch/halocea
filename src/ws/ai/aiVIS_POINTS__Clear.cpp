#include "../../headers/ws/ai/aiVIS_POINTS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // m3dVUndef / m3dVZero

// aiVIS_POINTS::Clear @ 0x83290C68
// ?Clear@aiVIS_POINTS@@QAAXXZ
// Reset the sampled sight points: the four world-space points to the undefined (NaN) sentinel and
// the four local-coordinate-space points to zero.
void aiVIS_POINTS::Clear()
{
    posBasic  = m3dVUndef;
    posBottom = m3dVUndef;
    posRandom = m3dVUndef;
    posTop    = m3dVUndef;
    posBasicLCS  = m3dVZero;
    posBottomLCS = m3dVZero;
    posRandomLCS = m3dVZero;
    posTopLCS    = m3dVZero;
}
