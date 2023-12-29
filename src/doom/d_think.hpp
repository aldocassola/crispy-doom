//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//  MapObj data. Map Objects or mobjs are actors, entities,
//  thinker, take-your-pick... anything that moves, acts, or
//  suffers state changes of more or less violent nature.
//


#ifndef __D_THINK__
#define __D_THINK__

#include <variant>


struct mobj_t;
struct player_t;
struct pspdef_t;
struct ceiling_t;
struct vldoor_t;
struct floormove_t;
struct plat_t;
struct lightflash_t;
struct strobe_t;
struct glow_t;
struct fireflicker_t;
struct thinker_t;

//
// Experimental stuff.
// To compile this as "ANSI C with classes"
//  we will need to handle the various
//  action functions cleanly.
//
typedef void (*actionf_v)();
typedef void (*actionf_p1)( mobj_t * );
typedef void (*actionf_p2)( mobj_t *, player_t * );
typedef void (*actionf_p3)( mobj_t *, player_t *, pspdef_t * ); // [crispy] let pspr action pointers get called from mobj states
using actionf_ceiling = void(*)(ceiling_t *);
using actionf_door = void(*)(vldoor_t *);
using actionf_floor = void(*)(floormove_t *);
using actionf_plat = void(*)(plat_t *);
using actionf_lightflash = void(*)(lightflash_t *);
using actionf_strobe = void(*)(strobe_t *);
using actionf_glow = void(*)(glow_t *);
using actionf_fireflicker = void(*)(fireflicker_t *);
using actionf_thinker = void(*)(thinker_t *);
using actionf_t = std::variant<
    std::monostate,
    actionf_v,
    actionf_p1,
    actionf_p2,
    actionf_p3,
    actionf_ceiling,
    actionf_door,
    actionf_floor,
    actionf_plat,
    actionf_lightflash,
    actionf_strobe,
    actionf_glow,
    actionf_fireflicker,
    actionf_thinker,
    bool
    >;


// Historically, "think_t" is yet another
//  function pointer to a routine to handle
//  an actor.
typedef actionf_t  think_t;


// Doubly linked list of actors.
struct thinker_t
{
    struct thinker_t*	prev;
    struct thinker_t*	next;
    think_t		function;

};



#endif
