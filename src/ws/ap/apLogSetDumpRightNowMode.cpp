// apLogSetDumpRightNowMode @0x825C8560 — set the ap-log "dump right now" flag, which forces log
// output to be flushed immediately rather than buffered.
extern "C" bool dumpRightNow; // boundary — ap-log module-global flag at 0x842090D0

void apLogSetDumpRightNowMode(bool rc)
{
    dumpRightNow = rc;
}
