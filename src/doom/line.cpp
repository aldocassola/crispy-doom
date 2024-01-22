#include <algorithm>

#include "line.hpp"
#include "vertex.hpp"
#include "doomdata.hpp"
#include "i_swap.h"
#include "m_fixed.h"
#include "m_bbox.h"
#include "r_state.hpp"

slopetype_t line_t::get_slope( fixed_t dx, fixed_t dy ) {
	if ( dx == 0 )
		return ST_VERTICAL;
	else if ( dy == 0 )
		return ST_HORIZONTAL;
	else if ( FixedDiv( dy, dx ) > 0 )
		return ST_POSITIVE;
	else
		return ST_NEGATIVE;
}

line_t::line_t( const maplinedef_t &mld )
	:
	v1( &vertexes[SHORT( mld.v1 )] ),
	v2( &vertexes[SHORT( mld.v2 )] ),
	dx( v2->x - v1->x ),
	dy( v2->y - v1->y ),
	flags( SHORT( mld.flags ) ),
	special( SHORT( mld.special ) ), // [crispy] extended nodes)
	tag( SHORT( mld.tag ) ),
	sidenum{ static_cast<unsigned short>( SHORT( mld.sidenum[0] ) ), static_cast<unsigned short>( SHORT( mld.sidenum[1] ) ) },
	slopetype( line_t::get_slope( dx, dy ) )
{
	if ( v1->x < v2->x )
	{
		bbox[BOXLEFT] = v1->x;
		bbox[BOXRIGHT] = v2->x;
	}
	else
	{
		bbox[BOXLEFT] = v2->x;
		bbox[BOXRIGHT] = v1->x;
	}

	if ( v1->y < v2->y )
	{
		bbox[BOXBOTTOM] = v1->y;
		bbox[BOXTOP] = v2->y;
	}
	else
	{
		bbox[BOXBOTTOM] = v2->y;
		bbox[BOXTOP] = v1->y;
	}

	// [crispy] calculate sound origin of line to be its midpoint
	soundorg.x = bbox[BOXLEFT] / 2 + bbox[BOXRIGHT] / 2;
	soundorg.y = bbox[BOXTOP] / 2 + bbox[BOXBOTTOM] / 2;


}
