#include "../../headers/ws/gs/gsiCONTEXT.h"
#include "../../headers/ws/ds/dsSTRID.h"

// @0x823CDAB0 — intern the command name (existOnly = true, so an unregistered name yields an
// invalid/empty dsSTRID) and forward to the dsSTRID overload.
bool gsiCONTEXT::IsJustOn(const char *cmd)
{
    dsSTRID cmdId(cmd, true);
    return IsJustOn(cmdId);
}
