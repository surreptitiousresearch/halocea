#pragma once
/* Effect node designator: bit 15 selects the node source (1 = a node on the local player's
 * first-person weapon, 0 = a node on the effect's world object), the low 15 bits are the node
 * index. The value -1 / 0xFFFF is the "no node" sentinel and stays a raw compare (locked
 * convention). Basis: every effect consumer tests bit 15 to route between
 * first_person_weapon_get_node_matrix and object_get_node_matrix, then masks the index with
 * 0x7FFF. */

#define EFFECT_NODE_FIRST_PERSON_WEAPON_FLAG    0x8000
#define EFFECT_NODE_IS_FIRST_PERSON_WEAPON(d)   (((d) & EFFECT_NODE_FIRST_PERSON_WEAPON_FLAG) != 0)
#define EFFECT_NODE_DESIGNATOR_TO_INDEX(d)      ((d) & 0x7FFF)
