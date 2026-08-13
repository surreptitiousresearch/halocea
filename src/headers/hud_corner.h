/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#ifndef HUD_CORNER_H
#define HUD_CORNER_H

/* hud_corner — screen-corner anchor for a directly-placed HUD element
 * (hud_absolute_placement_definition.corner @ +0x00).
 *
 * STILL-ABSENT as a DB enum (re-checked 2026-07-20 BLOCKED-retry): the DB $D0F641CA match on
 * "hud_corner" is a FALSE POSITIVE — it is the member _hs_type_enum_hud_corner inside the HaloScript
 * hs_type enum, not a hud_corner value set. A precise search (value_name LIKE '%hud_corner_top%' /
 * '%corner_bottom%') returns nothing. Kept as the .data-label + hud_calculate_point reconstruction.
 * Names recovered from the binary's
 * own data: this is one of the four HaloScript-facing enums whose ordered name-string table lives in
 * .data and is registered in the hs enum-type block @0x8211C9C4. The hud_corner label table is
 * @0x84185368: "top_left"[0], "top_right"[1], "bottom_left"[2], "bottom_right"[3], "center"[4].
 *
 * Cross-validated independently by the code itself: hud_calculate_point @0x8379E110 decodes `corner`
 * bit-wise — bit 0 selects the right vs left window edge, bit 1 selects bottom vs top — so 0=top-left,
 * 1=top-right, 2=bottom-left, 3=bottom-right, and corner>=4 (== center) takes the centered branch.
 * That bit layout matches the label table exactly. */
enum hud_corner {
	_hud_corner_top_left = 0,
	_hud_corner_top_right = 1,
	_hud_corner_bottom_left = 2,
	_hud_corner_bottom_right = 3,
	_hud_corner_center = 4,
	NUMBER_OF_HUD_CORNERS = 5,
};

#endif /* HUD_CORNER_H */
// hud_corner: the label table cited above is hs_enum_table @0x8211C9C4 record 4 —
// the registration block was fully decoded 2026-08-13 (enum drain round 2), confirming
// count=5 and the order top_left/top_right/bottom_left/bottom_right/center.
