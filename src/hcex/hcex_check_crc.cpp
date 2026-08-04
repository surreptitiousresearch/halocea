/* hcex_check_crc @0x823BFAB8 — co-op desync check: collect this tick's CRC, then, if the local
 * message handler has a queued raw-input tick from the remote peer, compare that remote tick's
 * reported CRC against our own history at the matching depth (hcex_data_applied_num -
 * lastRawInput.tick ticks back). A mismatch logs the two CRCs and, in debug builds, hard-asserts
 * ("crc deviation").
 *
 * coopMsgHandler is the ws/Blam co-op networking bridge (mp::MESSAGE_HANDLER-derived) — an extern
 * boundary; only the two fields this function reads (lastRawInput.tick, lastRawInput.crc) are
 * modeled at their database offsets. */

#include "../headers/ws/ds/dsCONST_ARRAY.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

/* COOP_MSG_HANDLER::lastRawInput (haloRAW_INPUT_X360_ENV, DB-verified): base haloRAW_INPUT_X360
 * (12B) @0, msgId@12, tick@16, crc@20 — only tick/crc are read here. */
typedef struct haloRAW_INPUT_X360_ENV
{
    unsigned char _base[12]; /* haloRAW_INPUT_X360 (buttons, thumbstick axes) — not touched here */
    int           msgId;     /* 0x0C */
    int           tick;      /* 0x10 */
    int           crc;       /* 0x14 */
} haloRAW_INPUT_X360_ENV;

/* COOP_MSG_HANDLER (DB-verified): mp::MESSAGE_HANDLER base @0 (4B), detInput @4 (32B), lastRawInput
 * @36. Only lastRawInput is touched here. */
typedef struct COOP_MSG_HANDLER
{
    unsigned char           _base_and_detInput[36]; /* mp::MESSAGE_HANDLER + dsDEQUE<...> — not touched here */
    haloRAW_INPUT_X360_ENV  lastRawInput;           /* 0x24 */
} COOP_MSG_HANDLER;

extern COOP_MSG_HANDLER       coopMsgHandler;
extern dsCONST_ARRAY<int, 64> crc_list;
extern int                    hcex_data_applied_num;
extern const char             dsStrongAssertMessage[]; /* empty_string — empty string */

extern void hcex_collect_crc(void);
extern void osOutputDebugString(const char *fmt, ...);

void hcex_check_crc(void)
{
    hcex_collect_crc();

    if (!hcex_data_applied_num)
        return;

    if (!coopMsgHandler.lastRawInput.tick)
        return;

    int ticksAgo = hcex_data_applied_num - coopMsgHandler.lastRawInput.tick;
    if (ticksAgo >= 0 && crc_list[ticksAgo] != coopMsgHandler.lastRawInput.crc) {
        osOutputDebugString(
            "0tick %d 1tick %d 0crc 0x%x 1crc 0x%x\n",
            hcex_data_applied_num,
            coopMsgHandler.lastRawInput.tick,
            crc_list[ticksAgo],
            coopMsgHandler.lastRawInput.crc);

        /* DEVIATION: the decompiler also shows an inner `if ( "crc deviation" )` guard around the
         * Crash call — a vacuous always-true test on a string-literal address, an artifact of how
         * the STRONG_ASSERT macro's condition check got folded into this already-taken branch.
         * Elided; the assert fires unconditionally once IGNORE_STRONG_ASSERT is false. */
        if (!IGNORE_STRONG_ASSERT) {
            STRONG_ASSERT_DUMMY().Crash(
                "!\"crc deviation\"",
                "D:\\Projects\\code\\HCEX\\sources\\halo_coop.cpp",
                244,
                dsStrongAssertMessage);
        }
    }
}
