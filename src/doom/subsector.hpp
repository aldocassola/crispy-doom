#pragma once
#ifndef _SUBSECTOR_HPP_
#define _SUBSECTOR_HPP_

#include "sector.hpp"

struct mapsubsector_t;

//
// A SubSector.
// References a Sector.
// Basically, this is a list of LineSegs,
//  indicating the visible walls that define
//  (all or some) sides of a convex BSP leaf.
//
struct subsector_t
{
    sector_t*	sector;
    int	numlines; // [crispy] extended nodes
    int	firstline; // [crispy] extended nodes

    subsector_t() = default;
    explicit subsector_t(const mapsubsector_t &);
};
#endif //_SUBSECTOR_HPP_