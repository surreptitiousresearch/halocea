#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ent/entENTITY.h"

// dsSPrintf — format into a fresh dsTSTRING<char> (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

// Append a NUL-terminated C string to the end of `line`.
static inline void AppendCStr(dsTSTRING<char> &line, const char *s)
{
    line.Insert(line.pBuffer->strLen, s, -1);
}

// Append another string to the end of `line`.
static inline void AppendStr(dsTSTRING<char> &line, const dsTSTRING<char> &s)
{
    line.Insert(line.pBuffer->strLen, s);
}

// aiGRASS::DbgGrenadesLine @ 0x83183F80
// ?DbgGrenadesLine@aiGRASS@@QAAXAAV?$dsTSTRING@D@@@Z
//
// Append a debug dump of the tracked grenades and smoke-out victims to `line`, of the form
//   "<count> [ grenadeName(shooterName), ... ]\r\n\tvictims: [ entityName, ... ]"
void aiGRASS::DbgGrenadesLine(dsTSTRING<char> &line)
{
    AppendStr(line, dsSPrintf("%d [", this->grenades.length));

    for (ds::LIST<GRENADE_INFO>::DATA *node = this->grenades.head; node; node = node->next)
    {
        AppendStr(line, const_cast<entENTITY *>(node->val.grenade)->GetName());

        entENTITY *shooter = node->val.shooter.pHandle ? node->val.shooter.pHandle->pPtr : nullptr;
        if (node->val.shooter.pHandle && shooter)
        {
            AppendCStr(line, "(");
            AppendStr(line, shooter->GetName());
            AppendCStr(line, ")");
        }
        AppendCStr(line, ", ");
    }
    AppendCStr(line, "]");

    AppendCStr(line, "\r\n\tvictims: [");
    for (ds::LIST<aiWATCHER *>::DATA *node = this->victims.head; node; node = node->next)
    {
        aiWATCHER *watcher = node->val;
        if (!watcher)
            continue;
        entENTITY *entity = watcher->GetEntity();
        if (!entity)
            continue;
        AppendStr(line, entity->GetName());
        AppendCStr(line, ", ");
    }
    AppendCStr(line, "]");
}
