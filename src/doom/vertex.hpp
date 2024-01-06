#pragma once
#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "m_fixed.hpp"
#include "doomtype.hpp"

struct mapvertex_t;

//
// Your plain vanilla vertex.
// Note: transformed values not buffered locally,
//  like some DOOM-alikes ("wt", "WebView") did.
//
struct vertex_t
{
    fixed_t	x;
    fixed_t	y;

// [crispy] remove slime trails
// vertex coordinates *only* used in rendering that have been
// moved towards the linedef associated with their seg by projecting them
// using the law of cosines in p_setup.c:P_RemoveSlimeTrails();
    fixed_t	r_x;
    fixed_t	r_y;
    boolean	moved;

    vertex_t() = default;
    vertex_t(const mapvertex_t &);
};

#endif //_VERTEX_HPP_