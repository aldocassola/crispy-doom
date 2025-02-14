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
//	Archiving: SaveGame I/O.
//	Thinker, Ticker.
//

#include <iostream>
#include "z_zone.h"
#include "p_local.hpp"
#include "s_musinfo.hpp" // [crispy] T_MAPMusic()

#include "doomstat.hpp"


int	leveltime;

//
// THINKERS
// All thinkers should be allocated by Z_Malloc
// so they can be operated on uniformly.
// The actual structures will vary in size,
// but the first element must be thinker_t.
//



// Both the head and tail of the thinker list.
thinker_t	thinkercap;


//
// P_InitThinkers
//
void P_InitThinkers (void)
{
    thinkercap.prev = thinkercap.next  = &thinkercap;
}




//
// P_AddThinker
// Adds a new thinker at the end of the list.
//
void P_AddThinker (thinker_t* thinker)
{
    thinkercap.prev->next = thinker;
    thinker->next = &thinkercap;
    thinker->prev = thinkercap.prev;
    thinkercap.prev = thinker;
}



//
// P_RemoveThinker
// Deallocation is lazy -- it will not actually be freed
// until its thinking turn comes up.
//
void P_RemoveThinker (thinker_t* thinker)
{
  // FIXME: NOP.
  thinker->function = -1;
}



//
// P_AllocateThinker
// Allocates memory and adds a new thinker at the end of the list.
//
void P_AllocateThinker (thinker_t*	thinker)
{
}



//
// P_RunThinkers
//
void P_RunThinkers (void)
{
    thinker_t *currentthinker, *nextthinker;

    currentthinker = thinkercap.next;
    while (currentthinker != &thinkercap)
    {
	if (auto ptr{std::get_if<int>(&currentthinker->function)}; ptr && *ptr == -1)
	{
	    // time to remove it
            nextthinker = currentthinker->next;
	    currentthinker->next->prev = currentthinker->prev;
	    currentthinker->prev->next = currentthinker->next;
	    Z_Free(currentthinker);
	}
	else
	{
		std::visit([&currentthinker](auto &&action) {
            using T = std::decay_t<decltype(action)>;

            if constexpr (std::is_same_v<T, actionf_p1>){
                action(reinterpret_cast<mobj_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_ceiling>) {
                action(reinterpret_cast<ceiling_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_door>) {
                action(reinterpret_cast<vldoor_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_floor>) {
                action(reinterpret_cast<floormove_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_plat>) {
                action(reinterpret_cast<plat_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_glow>) {
                action(reinterpret_cast<glow_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_strobe>) {
                action(reinterpret_cast<strobe_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_fireflicker>) {
                action(reinterpret_cast<fireflicker_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_lightflash>) {
                action(reinterpret_cast<lightflash_t *>(currentthinker));
            } else if constexpr (std::is_same_v<T, actionf_thinker>) {
                action(reinterpret_cast<thinker_t *>(currentthinker));
            } else if constexpr(std::is_same_v<T, std::monostate>) {
                //noop;
            } else {
                std::cerr << __func__ << ": unhandled function type for thinker:" << currentthinker << " func:" << typeid(action).name() << "\n";
            }
        }, currentthinker->function);
        nextthinker = currentthinker->next;
	}
	currentthinker = nextthinker;
    }

    // [crispy] support MUSINFO lump (dynamic music changing)
    T_MusInfo();
}



//
// P_Ticker
//

void P_Ticker (void)
{
    int		i;

    // run the tic
    if (paused)
	return;

    // pause if in menu and at least one tic has been run
    if ( !netgame
	 && menuactive
	 && !demoplayback
	 && players[consoleplayer].viewz != 1)
    {
	return;
    }


    for (i=0 ; i<MAXPLAYERS ; i++)
	if (playeringame[i])
	    P_PlayerThink (&players[i]);

    P_RunThinkers ();
    P_UpdateSpecials ();
    P_RespawnSpecials ();

    // for par times
    leveltime++;
}
