#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated no-DB layout; evidence and falsifier below.
 *
 * animation_compressed_block — header of an animation's compressed keyframe blob, located at
 * (char *)animation.data.address + animation.compressed_data_offset. Eleven uint32 words, each a
 * BYTE OFFSET measured from the start of this header, locate the per-channel data regions. The
 * rotation channel has no descriptor-table offset word: its per-node descriptor table is stored
 * inline immediately after the eleven words, at 0x2C, and is addressed directly.
 *
 * EVIDENCE — three consumers, and they are the only ones:
 *
 *   animation_get_keyframe_rotation    @0x83796390
 *   animation_get_keyframe_translation @0x837965B8
 *   animation_get_keyframe_scale       @0x83796800
 *
 * Enumerated, not assumed. Every function over 0x83600000-0x83900000 that loads BOTH of the two
 * displacements this base is built from — animation+0x88 (compressed_data_offset) and
 * animation+0xAC (data.address) — was listed from the instruction table; the only animation-module
 * members of that list are these three plus animation_get_frame_data @0x838186F0, which merely
 * forms and returns the base pointer and never indexes it. (The remaining matches on that raw
 * displacement pair address unrelated structures: FMOD EventI, observer, biped, hkpWorld …)
 *
 * Between them these three load exactly eleven distinct fixed displacements off the block base
 * (r11 in all three prologues), with no overlap and no gaps, plus the inline table:
 *
 *   0x00  lwz  r7,  0(r11)     @0x83796424       0x1C  lwz  r10, 0x1C(r11)  @0x8379682C
 *   0x04  lwz  r10, 4(r11)     @0x837963C8       0x20  lwz  r8,  0x20(r11)  @0x83796870
 *   0x08  lwz  r9,  8(r11)     @0x83796420       0x24  lwz  r9,  0x24(r11)  @0x83796830
 *   0x0C  lwz  r10, 0xC(r11)   @0x837965E8       0x28  lwz  r9,  0x28(r11)  @0x8379686C
 *   0x10  lwz  r7,  0x10(r11)  @0x83796650
 *   0x14  lwz  r8,  0x14(r11)  @0x837965EC       0x2C  addi r9, r11, 0xB ; slwi r8, r9, 2 ;
 *   0x18  lwz  r9,  0x18(r11)  @0x8379664C             lwzx r7, r8, r11  @0x837963B4-0x837963CC
 *
 * That last form is what settles the count. The rotation descriptor of node 0 is word 11, read
 * with no indirection through an offset word at all, so the offset-word run is exactly words
 * 0..10 — eleven words spanning 0x00..0x28 inclusive — and the descriptor table begins at 0x2C.
 *
 * Each field's ROLE is pinned by the stride the loaded value is subsequently indexed with, not by
 * guesswork: 2 = an int16 frame-index table, 4 = a uint32 descriptor or a float scale, 6 = a
 * compressed_quaternion_6byte, 12 = a real_point3d. Per-field instruction cites are on the members.
 *
 * NO DB TYPE backs this. types_members holds no row for any type that models it: every '%compress%'
 * and '%keyframe%' hit is Havok, D3DX, XCOMPRESS, FMOD, NUISPEECH or a vertex format, and
 * `_animation_data` is the unrelated four-int16 bitmap sprite-animation record. Re-checked
 * 2026-08-06 against the live database.
 *
 * WOULD BE FALSIFIED BY: a further consumer — a tag importer, an endian-swap or postprocess pass,
 * a compressor — that reads any of words 0..10 with a different element stride, or that treats
 * word 11 as a twelfth offset word rather than as the first rotation descriptor. No such function
 * exists in this image.
 *
 * A channel DESCRIPTOR word packs, for one node:
 *   bits  0..11  keyframe count on this channel (clrlwi rX, rY, 20 — 0x837963D4, 0x837965FC,
 *                0x83796840); zero means the node is not animated on the channel and the default
 *                table supplies its value
 *   bits 12..27  index of the node's first keyframe within the channel's frame-index and value
 *                tables (extrwi rX, rY, 16, 4 — 0x837963D8, 0x83796600, 0x83796844), consumed
 *                sign-extended as an int16
 *   bits 28..31  never read by any consumer
 * Bit numbers are LSB=0.
 */

#include <stdint.h>

typedef struct animation_compressed_block
{
    /* rotation channel — its descriptor table is the inline array at 0x2C, below */
    uint32_t rotation_frame_index_offset;     /* 0x00 — int16 frame-index table, indexed by the
                                               * descriptor's data index (2-byte stride, 0x83796438) */
    uint32_t rotation_default_offset;         /* 0x04 — per-node default orientation, one
                                               * compressed_quaternion_6byte per node, indexed by
                                               * node_index (6-byte stride, 0x837963F0-0x837963FC) */
    uint32_t rotation_keyframe_offset;        /* 0x08 — keyframe orientations, one
                                               * compressed_quaternion_6byte each, indexed by the
                                               * data index (6-byte stride, 0x8379642C-0x83796448) */

    /* translation channel */
    uint32_t translation_descriptor_offset;   /* 0x0C — one uint32 descriptor per node, indexed by
                                               * adjusted_node_index (4-byte stride, 0x837965D8) */
    uint32_t translation_frame_index_offset;  /* 0x10 — int16 frame-index table (2-byte stride,
                                               * 0x8379665C) */
    uint32_t translation_default_offset;      /* 0x14 — per-node default translation, real_point3d,
                                               * indexed by node_index (12-byte stride, 0x8379661C) */
    uint32_t translation_keyframe_offset;     /* 0x18 — keyframe translations, real_point3d, indexed
                                               * by the data index (12-byte stride, 0x83796668) */

    /* scale channel */
    uint32_t scale_descriptor_offset;         /* 0x1C — one uint32 descriptor per node, indexed by
                                               * adjusted_node_index (4-byte stride, 0x83796820) */
    uint32_t scale_frame_index_offset;        /* 0x20 — int16 frame-index table (2-byte stride,
                                               * 0x8379687C) */
    uint32_t scale_default_offset;            /* 0x24 — per-node default scale, float, indexed by
                                               * adjusted_node_index (lfsx r28, r26 @0x83796854) */
    uint32_t scale_keyframe_offset;           /* 0x28 — keyframe scales, float, indexed by the data
                                               * index (4-byte stride, 0x83796878) */

    /* 0x2C — rotation descriptor table, stored inline rather than located by an offset word: one
     * uint32 per node, indexed by adjusted_node_index (0x837963B4-0x837963CC). Declared [1]
     * because the real length is the animation's node_count, which the blob does not restate. */
    uint32_t rotation_descriptors[1];
} animation_compressed_block;
