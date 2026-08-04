#include "headers/ws/rend/SWIZZLER.h"

// 0x82D580C4 -- assigns bits round-robin to U/V/W: each round, every axis whose dimension still
// exceeds the round's power-of-two `threshold` claims the next `bit` (shared cursor across all
// three axes) into its mask; stops once a round claims no bits.
void SWIZZLER::Init(unsigned int width, unsigned int height, unsigned int depth)
{
    this->m_Width = width;
    this->m_Height = height;
    this->m_Depth = depth;
    this->m_MaskU = 0;
    this->m_MaskV = 0;
    this->m_MaskW = 0;
    this->m_u = 0;
    this->m_v = 0;
    this->m_w = 0;

    unsigned int threshold = 1;
    unsigned int bit = 1;
    unsigned int claimedThisRound;

    do
    {
        claimedThisRound = 0;

        if (threshold < width)
        {
            this->m_MaskU |= bit;
            bit *= 2;
            claimedThisRound = bit;
        }
        if (threshold < height)
        {
            this->m_MaskV |= bit;
            bit *= 2;
            claimedThisRound = bit;
        }
        if (threshold < depth)
        {
            this->m_MaskW |= bit;
            bit *= 2;
            claimedThisRound = bit;
        }

        threshold *= 2;
    } while (claimedThisRound);
}
