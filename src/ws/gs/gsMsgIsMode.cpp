// gsMsgIsMode @0x82507BA0 — test whether any of the message-system mode bits in `mode` are
// currently set. Returns the masked bits (nonzero iff at least one requested bit is active).
extern unsigned int gsMsgMode; // boundary — gs message-system mode bitset (module-global)

unsigned int gsMsgIsMode(unsigned int mode)
{
    return gsMsgMode & mode;
}
