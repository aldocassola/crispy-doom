//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
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

// DStrings.h

//---------------------------------------------------------------------------
//
// P_inter.c
//
//---------------------------------------------------------------------------

// Keys

#define TXT_GOTBLUEKEY			"BLUE KEY"
#define TXT_GOTYELLOWKEY		"YELLOW KEY"
#define TXT_GOTGREENKEY			"GREEN KEY"

// Artifacts

#define TXT_ARTIHEALTH			"QUARTZ FLASK"
#define TXT_ARTIFLY				"WINGS OF WRA.h"
#define TXT_ARTIINVULNERABILITY	"RING OF INVINCIBILITY"
#define TXT_ARTITOMEOFPOWER		"TOME OF POWER"
#define TXT_ARTIINVISIBILITY	"SHADOWSPHERE"
#define TXT_ARTIEGG				"MORPH OVUM"
#define TXT_ARTISUPERHEALTH		"MYSTIC URN"
#define TXT_ARTITORCH			"TOR.h"
#define TXT_ARTIFIREBOMB		"TIME BOMB OF THE ANCIENTS"
#define TXT_ARTITELEPORT		"CHAOS DEVICE"

// Items

#define TXT_ITEMHEALTH			"CRYSTAL VIAL"
#define TXT_ITEMBAGOFHOLDING	"BAG OF HOLDING"
#define TXT_ITEMSHIELD1			"SILVER SHIELD"
#define TXT_ITEMSHIELD2			"ENCHANTED SHIELD"
#define TXT_ITEMSUPERMAP		"MAP SCROLL"

// Ammo

#define TXT_AMMOGOLDWAND1		"WAND CRYSTAL"
#define TXT_AMMOGOLDWAND2		"CRYSTAL GEODE"
#define TXT_AMMOMACE1			"MACE SPHERES"
#define TXT_AMMOMACE2			"PILE OF MACE SPHERES"
#define TXT_AMMOCROSSBOW1		"ETHEREAL ARROWS"
#define TXT_AMMOCROSSBOW2		"QUIVER OF ETHEREAL ARROWS"
#define TXT_AMMOBLASTER1		"CLAW ORB"
#define TXT_AMMOBLASTER2		"ENERGY ORB"
#define TXT_AMMOSKULLROD1		"LESSER RUNES"
#define TXT_AMMOSKULLROD2		"GREATER RUNES"
#define TXT_AMMOPHOENIXROD1		"FLAME ORB"
#define TXT_AMMOPHOENIXROD2		"INFERNO ORB"

// Weapons

#define TXT_WPNMACE				"FIREMACE"
#define TXT_WPNCROSSBOW			"ETHEREAL CROSSBOW"
#define TXT_WPNBLASTER			"DRAGON CLAW"
#define TXT_WPNSKULLROD			"HELLSTAFF"
#define TXT_WPNPHOENIXROD		"PHOENIX ROD"
#define TXT_WPNGAUNTLETS		"GAUNTLETS OF THE NECROMANCER"

//---------------------------------------------------------------------------
//
// SB_bar.c
//
//---------------------------------------------------------------------------

#define TXT_CHEATGODON			"GOD MODE ON"
#define TXT_CHEATGODOFF			"GOD MODE OFF"
#define TXT_CHEATNOCLIPON		"NO CLIPPING ON"
#define TXT_CHEATNOCLIPOFF		"NO CLIPPING OFF"
#define TXT_CHEATWEAPONS		"ALL WEAPONS"
#define TXT_CHEATFLIGHTON		"FLIGHT ON"
#define TXT_CHEATFLIGHTOFF		"FLIGHT OFF"
#define TXT_CHEATPOWERON		"POWER ON"
#define TXT_CHEATPOWEROFF		"POWER OFF"
#define TXT_CHEATHEALTH			"FULL HEAL.h"
#define TXT_CHEATKEYS			"ALL KEYS"
#define TXT_CHEATSOUNDON		"SOUND DEBUG ON"
#define TXT_CHEATSOUNDOFF		"SOUND DEBUG OFF"
#define TXT_CHEATTICKERON		"TICKER ON"
#define TXT_CHEATTICKEROFF		"TICKER OFF"
#define TXT_CHEATARTIFACTS1		"CHOOSE AN ARTIFACT ( A - J )"
#define TXT_CHEATARTIFACTS2		"HOW MANY ( 1 - 9 )"
#define TXT_CHEATARTIFACTS3		"YOU GOT IT"
#define TXT_CHEATARTIFACTSFAIL	"BAD INPUT"
#define TXT_CHEATWARP			"LEVEL WARP"
#define TXT_CHEATSCREENSHOT		"SCREENSHOT"
#define TXT_CHEATCHICKENON		"CHICKEN ON"
#define TXT_CHEATCHICKENOFF		"CHICKEN OFF"
#define TXT_CHEATMASSACRE		"MASSACRE"
#define TXT_CHEATIDDQD			"TRYING TO CHEAT, EH?  NOW YOU DIE!"
#define TXT_CHEATIDKFA			"CHEATER - YOU DON'T DESERVE WEAPONS"

// [crispy] strings for new cheats
#define TXT_SHOWFPSON			"SHOW FPS ON"
#define TXT_SHOWFPSOFF			"SHOW FPS OFF"
#define TXT_CHEATNOTARGETON		"NOTARGET MODE ON"
#define TXT_CHEATNOTARGETOFF		"NOTARGET MODE OFF"
#define TXT_CHEATWEAPREMOVEALL		"ALL WEAPONS REMOVED"
#define TXT_CHEATWEAPBAD		"WEAPON NOT AVAILABLE"
#define TXT_CHEATWEAPADD		"WEAPON %i ADDED"
#define TXT_CHEATWEAPREMOVE		"WEAPON %i REMOVED"
#define TXT_CHEATSPECHIT		"%i SPECIAL LINE%s TRIGGERED"
#define TXT_CHEATNOMOMON		"NO MOMENTUM MODE ON"
#define TXT_CHEATNOMOMOFF		"NO MOMENTUM MODE OFF"
#define TXT_HOMDETECTON			"HOM DETECTION ON"
#define TXT_HOMDETECTOFF		"HOM DETECTION OFF"

// [crispy] gamma correction messages
#define TXT_GAMMA_LEVEL_050		"GAMMA CORRECTION LEVEL -4"
#define TXT_GAMMA_LEVEL_055		"GAMMA CORRECTION LEVEL -3.6"
#define TXT_GAMMA_LEVEL_060		"GAMMA CORRECTION LEVEL -3.2"
#define TXT_GAMMA_LEVEL_065		"GAMMA CORRECTION LEVEL -2.8"
#define TXT_GAMMA_LEVEL_070		"GAMMA CORRECTION LEVEL -2.4"
#define TXT_GAMMA_LEVEL_075		"GAMMA CORRECTION LEVEL -2.0"
#define TXT_GAMMA_LEVEL_080		"GAMMA CORRECTION LEVEL -1.6"
#define TXT_GAMMA_LEVEL_085		"GAMMA CORRECTION LEVEL -1.2"
#define TXT_GAMMA_LEVEL_090		"GAMMA CORRECTION LEVEL -0.8"
#define TXT_GAMMA_LEVEL_OFF		"GAMMA CORRECTION OFF"
#define TXT_GAMMA_LEVEL_05		"GAMMA CORRECTION LEVEL 0.5"
#define TXT_GAMMA_LEVEL_1		"GAMMA CORRECTION LEVEL 1"
#define TXT_GAMMA_LEVEL_15		"GAMMA CORRECTION LEVEL 1.5"
#define TXT_GAMMA_LEVEL_2		"GAMMA CORRECTION LEVEL 2"
#define TXT_GAMMA_LEVEL_25		"GAMMA CORRECTION LEVEL 2.5"
#define TXT_GAMMA_LEVEL_3		"GAMMA CORRECTION LEVEL 3"
#define TXT_GAMMA_LEVEL_35		"GAMMA CORRECTION LEVEL 3.5"
#define TXT_GAMMA_LEVEL_4		"GAMMA CORRECTION LEVEL 4"

//---------------------------------------------------------------------------
//
// P_doors.c
//
//---------------------------------------------------------------------------

#define TXT_NEEDBLUEKEY			"YOU NEED A BLUE KEY TO OPEN THIS DOOR"
#define TXT_NEEDGREENKEY		"YOU NEED A GREEN KEY TO OPEN THIS DOOR"
#define TXT_NEEDYELLOWKEY		"YOU NEED A YELLOW KEY TO OPEN THIS DOOR"

//---------------------------------------------------------------------------
//
// G_game.c
//
//---------------------------------------------------------------------------

#define TXT_GAMESAVED			"GAME SAVED"

//---------------------------------------------------------------------------
//
// AM_map.c
//
//---------------------------------------------------------------------------

#define AMSTR_FOLLOWON		"FOLLOW MODE ON"
#define AMSTR_FOLLOWOFF		"FOLLOW MODE OFF"

#define AMSTR_GRIDON		"GRID ON"
#define AMSTR_GRIDOFF		"GRID OFF"

#define AMSTR_MARKEDSPOT	"Marked Spot"
#define AMSTR_MARKSCLEARED	"All Marks Cleared"

#define AMSTR_OVERLAYON		"OVERLAY MODE ON"
#define AMSTR_OVERLAYOFF	"OVERLAY MODE OFF"

#define AMSTR_ROTATEON		"ROTATE MODE ON"
#define AMSTR_ROTATEOFF		"ROTATE MODE OFF"

//---------------------------------------------------------------------------
//
// F_finale.c
//
//---------------------------------------------------------------------------

#define E1TEXT	"with the destruction of the iron\n"\
					"liches and their minions, the last\n"\
					"of the undead are cleared from this\n"\
					"plane of existence.\n\n"\
					"those creatures had to come from\n"\
					"somewhere, though, and you have the\n"\
					"sneaky suspicion that the fiery\n"\
					"portal of hell's maw opens onto\n"\
					"their home dimension.\n\n"\
					"to make sure that more undead\n"\
					"(or even worse things) don't come\n"\
					"through, you'll have to seal hell's\n"\
					"maw from the other side. of course\n"\
					"this means you may get stuck in a\n"\
					"very unfriendly world, but no one\n"\
					"ever said being a Heretic was easy!"

#define E2TEXT "the mighty maulotaurs have proved\n"\
					"to be no match for you, and as\n"\
					"their steaming corpses slide to the\n"\
					"ground you feel a sense of grim\n"\
					"satisfaction that they have been\n"\
					"destroyed.\n\n"\
					"the gateways which they guarded\n"\
					"have opened, revealing what you\n"\
					"hope is the way home. but as you\n"\
					"step through, mocking laughter\n"\
					"rings in your ears.\n\n"\
					"was some other force controlling\n"\
					"the maulotaurs? could there be even\n"\
					"more horrific beings through this\n"\
					"gate? the sweep of a crystal dome\n"\
					"overhead where the sky should be is\n"\
					"certainly not a good sign...."

#define E3TEXT	"the death of d'sparil has loosed\n"\
					"the magical bonds holding his\n"\
					"creatures on this plane, their\n"\
					"dying screams overwhelming his own\n"\
					"cries of agony.\n\n"\
					"your oath of vengeance fulfilled,\n"\
					"you enter the portal to your own\n"\
					"world, mere moments before the dome\n"\
					"shatters into a million pieces.\n\n"\
					"but if d'sparil's power is broken\n"\
					"forever, why don't you feel safe?\n"\
					"was it that last shout just before\n"\
					"his death, the one that sounded\n"\
					"like a curse? or a summoning? you\n"\
					"can't really be sure, but it might\n"\
					"just have been a scream.\n\n"\
					"then again, what about the other\n"\
					"serpent riders?"

#define E4TEXT		"you thought you would return to your\n"\
					"own world after d'sparil died, but\n"\
					"his final act banished you to his\n"\
					"own plane. here you entered the\n"\
					"shattered remnants of lands\n"\
					"conquered by d'sparil. you defeated\n"\
					"the last guardians of these lands,\n"\
					"but now you stand before the gates\n"\
					"to d'sparil's stronghold. until this\n"\
					"moment you had no doubts about your\n"\
					"ability to face anything you might\n"\
					"encounter, but beyond this portal\n"\
					"lies the very heart of the evil\n"\
					"which invaded your world. d'sparil\n"\
					"might be dead, but the pit where he\n"\
					"was spawned remains. now you must\n"\
					"enter that pit in the hopes of\n"\
					"finding a way out. and somewhere,\n"\
					"in the darkest corner of d'sparil's\n"\
					"demesne, his personal bodyguards\n"\
					"await your arrival ..."

#define E5TEXT		"as the final maulotaur bellows his\n"\
					"death-agony, you realize that you\n"\
					"have never come so close to your own\n"\
					"destruction. not even the fight with\n"\
					"d'sparil and his disciples had been\n"\
					"this desperate. grimly you stare at\n"\
					"the gates which open before you,\n"\
					"wondering if they lead home, or if\n"\
					"they open onto some undreamed-of\n"\
					"horror. you find yourself wondering\n"\
					"if you have the strength to go on,\n"\
					"if nothing but death and pain await\n"\
					"you. but what else can you do, if\n"\
					"the will to fight is gone? can you\n"\
					"force yourself to continue in the\n"\
					"face of such despair? do you have\n"\
					"the courage? you find, in the end,\n"\
					"that it is not within you to\n"\
					"surrender without a fight. eyes\n"\
					"wide, you go to meet your fate."

