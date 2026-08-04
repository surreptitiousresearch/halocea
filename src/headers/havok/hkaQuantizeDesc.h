#pragma once

/* types_members hkaQuantizeDesc -- size 12. Per-value quantization descriptor:
   bit width + preserved flag plus the scale/offset used to map a quantized
   integer back to a float. Consumed by hkCalcQuantizedSize / hkExpandReal. */
typedef struct hkaQuantizeDesc
{
    unsigned char m_bitWidth;  /* 0 */
    unsigned char m_preserved; /* 1 */
    float m_scale;             /* 4 */
    float m_offset;            /* 8 */
} hkaQuantizeDesc;
