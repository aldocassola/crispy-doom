//sector impl

#include "sector.hpp"
#include "r_data.hpp"
#include "i_system.hpp"
#include "i_swap.hpp"

sector_t::sector_t() {
    I_GetMemoryValue( 0, &floorheight, 4 );
    I_GetMemoryValue( 4, &ceilingheight, 4 );
};

sector_t::sector_t( const mapsector_t &ms )
    : floorheight( SHORT( ms.floorheight ) << FRACBITS ),
    ceilingheight( SHORT( ms.ceilingheight ) << FRACBITS ),
    floorpic( R_FlatNumForName( ms.floorpic ) ),
    ceilingpic( R_FlatNumForName( ms.ceilingpic ) ),
    lightlevel( SHORT( ms.lightlevel ) ),
    // [crispy] A11Y light level used for rendering
    special( ms.special ),
    tag( SHORT( ms.tag ) ),
    soundtraversed(0),
    soundtarget(nullptr),
    blockbox{},
    soundorg{},
    validcount(0),
    thinglist( nullptr ),
    specialdata(nullptr),
    linecount(0),
    lines(nullptr),
    // [crispy] WiggleFix: [kb] for R_FixWiggle()
    cachedheight( 0 ),
    scaleindex(0),
    sky(0),
    oldfloorheight( floorheight ),
    oldceilingheight( ceilingheight ),
    oldgametic( -1 ),
    interpfloorheight( floorheight ),
    interpceilingheight( ceilingheight ),
    oldspecial(0),
    rlightlevel( lightlevel )
{ }