#pragma once
#ifndef _SECTOR_HPP_
#define _SECTOR_HPP_

#include "m_fixed.h"
#include "p_mobj.hpp"

struct line_t;

//
// The SECTORS record, at runtime.
// Stores things/mobjs.
//
struct sector_t
{
    fixed_t	floorheight;
    fixed_t	ceilingheight;
    short	floorpic;
    short	ceilingpic;
    short	lightlevel;
    short	special;
    short	tag;

    // 0 = untraversed, 1,2 = sndlines -1
    int		soundtraversed;

    // thing that made a sound (or null)
    mobj_t*	soundtarget;

    // mapblock bounding box for height changes
    int		blockbox[4];

// Each sector has a degenmobj_t in its center
//  for sound origin purposes.
// I suppose this does not handle sound from
//  moving objects (doppler), because
//  position is prolly just buffered, not
//  updated.

    // origin for any sounds played by the sector
    degenmobj_t	soundorg;

    // if == validcount, already checked
    int		validcount;

    // list of mobjs in sector
    mobj_t*	thinglist;

    // thinker_t for reversable actions
    void*	specialdata;

    int			linecount;
    line_t**	lines;	// [linecount] size

    // [crispy] WiggleFix: [kb] for R_FixWiggle()
    int		cachedheight;
    int		scaleindex;

    // [crispy] add support for MBF sky tranfers
    int		sky;

    // [AM] Previous position of floor and ceiling before
    //      think.  Used to interpolate between positions.
    fixed_t	oldfloorheight;
    fixed_t	oldceilingheight;

    // [AM] Gametic when the old positions were recorded.
    //      Has a dual purpose; it prevents movement thinkers
    //      from storing old positions twice in a tic, and
    //      prevents the renderer from attempting to interpolate
    //      if old values were not updated recently.
    int         oldgametic;

    // [AM] Interpolated floor and ceiling height.
    //      Calculated once per tic and used inside
    //      the renderer.
    fixed_t	interpfloorheight;
    fixed_t	interpceilingheight;

    // [crispy] revealed secrets
    short	oldspecial;

    // [crispy] A11Y light level used for rendering
    short	rlightlevel;

    sector_t();
    explicit sector_t(const mapsector_t &ms);
};
#endif //_SECTOR_HPP_