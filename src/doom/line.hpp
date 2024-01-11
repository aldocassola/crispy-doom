#pragma once
#ifndef _LINE_HPP_
#define _LINE_HPP_

#include "m_fixed.h"
#include "p_mobj.hpp"

struct vertex_t;
struct sector_t;

//
// Move clipping aid for LineDefs.
//
enum slopetype_t
{
    ST_HORIZONTAL,
    ST_VERTICAL,
    ST_POSITIVE,
    ST_NEGATIVE
};

struct line_t
{
    // Vertices, from v1 to v2.
    vertex_t*	v1;
    vertex_t*	v2;

    // Precalculated v2 - v1 for side checking.
    fixed_t	dx;
    fixed_t	dy;

    // Animation related.
    unsigned short	flags; // [crispy] extended nodes
    short	special;
    short	tag;

    // Visual appearance: SideDefs.
    //  sidenum[1] will be -1 (NO_INDEX) if one sided
    unsigned short	sidenum[2]; // [crispy] extended nodes

    // Neat. Another bounding box, for the extent
    //  of the LineDef.
    fixed_t	bbox[4];

    // To aid move clipping.
    slopetype_t	slopetype;

    // Front and back sector.
    // Note: redundant? Can be retrieved from SideDefs.
    sector_t*	frontsector;
    sector_t*	backsector;

    // if == validcount, already checked
    int		validcount;

    // thinker_t for reversable actions
    void*	specialdata;

    // [crispy] calculate sound origin of line to be its midpoint
    degenmobj_t	soundorg;

    line_t() = default;
    explicit line_t(const maplinedef_t &);
    static slopetype_t get_slope(fixed_t dx, fixed_t dy);
};

#endif //_LINE_HPP_