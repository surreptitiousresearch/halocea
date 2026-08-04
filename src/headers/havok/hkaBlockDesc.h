#pragma once

/* types_members hkaBlockDesc -- size 12. Per-block run-length descriptor handed
   to hkBlockDecode: the symbol bit width, the "run" symbol (the quantized value
   that encodes an unchanged / zero sample), and the preserved flag. */
typedef struct hkaBlockDesc
{
    unsigned int m_bitWidth;  /* 0 */
    unsigned int m_runSymbol; /* 4 */
    unsigned int m_preserved; /* 8 */
} hkaBlockDesc;
