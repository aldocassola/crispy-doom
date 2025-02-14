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
//
//


#ifndef __D_PLAYER__
#define __D_PLAYER__


// The player data structure depends on a number
// of other structs: items (internal inventory),
// animation states (closely tied to the sprites
// used to represent them, unfortunately).
#include "d_items.h"
#include "p_pspr.h"

// In addition, the player is just a special
// case of the generic moving object/actor.
#include "p_mobj.h"

// Finally, for odd reasons, the player input
// is buffered within the player data struct,
// as commands per game tick.
#include "d_ticcmd.h"

#include "net_defs.h"




//
// Player states.
//
typedef enum
{
    // Playing or camping.
    PST_LIVE,
    // Dead on the ground, view follows killer.
    PST_DEAD,
    // Ready to restart/respawn???
    PST_REBORN

} playerstate_t;


//
// Player internal flags, for cheats and debug.
//
typedef enum
{
    // No clipping, walk through barriers.
    CF_NOCLIP		= 1,
    // No damage, no health loss.
    CF_GODMODE		= 2,
    // Not really a cheat, just a debug aid.
    CF_NOMOMENTUM	= 4,
    // villsa [STRIFE] new cheat
    // set when on fire and disable inventory
    CF_ONFIRE           = 8,
    // villsa [STRIFE] new cheat
    // auto-use medkits
    CF_AUTOHEALTH       = 16

} cheat_t;

// haleyjd 08/30/10: [STRIFE]
// Player Inventory Item Structure
typedef struct inventory_s
{
    int sprite; // a sprite number
    int type;   // a thing type
    int amount; // amount being carried
} inventory_t;

#define NUMINVENTORY 32

//
// Extended player object info: player_t
//
// haleyjd 08/30/10: [STRIFE]
// * Transformed to match binary structure layout.
//
typedef struct player_s
{
    mobj_t*		mo;
    playerstate_t	playerstate;
    ticcmd_t		cmd;

    // Determine POV,
    //  including viewpoint bobbing during movement.
    // Focal origin above r.z
    fixed_t		viewz;
    // Base height above floor for viewz.
    fixed_t		viewheight;
    // Bob/squat speed.
    fixed_t         	deltaviewheight;
    // bounded/scaled total momentum.
    fixed_t         	bob;

    // This is only used between levels,
    // mo->health is used during levels.
    int			health;
    short		armorpoints; // [STRIFE] Changed to short
    // Armor type is 0-2.
    short		armortype;   // [STRIFE] Changed to short

    // Power ups. invinc and invis are tic counters.
    int			powers[NUMPOWERS + 2]; // [crispy] showfps widget

    // [STRIFE] Additions:
    int			sigiltype;               // Type of Sigil carried
    int			nukagecount;             // Nukage exposure counter
    int			questflags;              // Quest bit flags
    int			pitch;                   // Up/down look angle
    boolean		centerview;              // True if view should be centered
    inventory_t		inventory[NUMINVENTORY]; // Player inventory items
    boolean		st_update;               // If true, update status bar
    short		numinventory;            // Num. active inventory items
    short		inventorycursor;         // Selected inventory item
    short		accuracy;                // Accuracy stat
    short		stamina;                 // Stamina stat

    boolean		cards[NUMCARDS];
    boolean		backpack;

    // True if button down last tic.
    int			attackdown;
    int			usedown;
    int			inventorydown;   // [STRIFE] Use inventory item

    // Frags, kills of other players.
    int			frags[MAXPLAYERS];
    weapontype_t	readyweapon;

    // Is wp_nochange if not changing.
    weapontype_t	pendingweapon;

    boolean		weaponowned[NUMWEAPONS];
    int			ammo[NUMAMMO];
    int			maxammo[NUMAMMO];

    // Bit flags, for cheats and debug.
    // See cheat_t, above.
    int			cheats;

    // Refired shots are less accurate.
    int			refire;

     // For intermission stats.
    short		killcount;    // [STRIFE] Changed to short
    //int		itemcount;    // [STRIFE] Eliminated these.
    //int		secretcount;

    // Hint messages.
    const char		*message;

    // For screen flashing (red or bright).
    int			damagecount;
    int			bonuscount;

    // Who did damage (NULL for floors/ceilings).
    mobj_t*		attacker;

    // So gun flashes light up areas.
    int			extralight;

    // Current PLAYPAL, ???
    //  can be set to REDCOLORMAP for pain, etc.
    int			fixedcolormap;

    // Player skin colorshift,
    //  0-3 for which color to draw player.
    //int			colormap; [STRIFE] no such? or did it become the below?

    // [STRIFE] For use of teleport beacons
    short		allegiance;

    // Overlay view sprites (gun, etc).
    pspdef_t		psprites[NUMPSPRITES];

    // [STRIFE] Inefficient means of tracking automap state on all maps
    boolean		mapstate[40];

    // True if secret level has been done.
    //boolean		didsecret;   [STRIFE] Removed this.

    // [AM] Previous position of viewz before think.
    //      Used to interpolate between camera positions.
    fixed_t		oldviewz;

    // [crispy] Previous up/down look angle.
    int			oldpitch;

    // [crispy] weapon sound source
    mobj_t*		so;

    // [crispy] variable player view bob
    fixed_t		bob2;

} player_t;


//
// INTERMISSION
// Structure passed e.g. to WI_Start(wb)
//
typedef struct
{
    boolean	in;	// whether the player is in game

    // Player stats, kills, collected items etc.
    int		skills;
    int		sitems;
    int		ssecret;
    int		stime;
    int		frags[4];
    int		score;	// current score on entry, modified on return

} wbplayerstruct_t;

typedef struct
{
    int		epsd;	// episode # (0-2)

    // if true, splash the secret level
    boolean	didsecret;

    // previous and next levels, origin 0
    int		last;
    int		next;

    int		maxkills;
    int		maxitems;
    int		maxsecret;
    int		maxfrags;

    // the par time
    int		partime;

    // index of this player in game
    int		pnum;

    wbplayerstruct_t	plyr[MAXPLAYERS];

} wbstartstruct_t;


#endif
