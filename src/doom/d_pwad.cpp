//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2020 Fabian Greffrath
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
//	Auto-loading of (semi-)official PWAD expansions, i.e.
//	Sigil, No Rest for the Living and The Master Levels
//

#include <cstdlib>
#include <string>
#include <memory>

#include "memory/memory.hpp"

#include "doomstat.hpp"
#include "deh_main.h"
#include "d_iwad.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "w_main.h"
#include "w_wad.h"
#include "w_merge.h" // [crispy] W_MergeFile()

extern char *iwadfile;

// [crispy] auto-load SIGIL.WAD (and SIGIL_SHREDS.WAD) if available
static boolean LoadSigilWad (const char *iwaddir, boolean pwadtexture)
{
	int j;
	const char *sigil_basename;

	const char *const sigil_wads[] = {
		"SIGIL_v1_21.wad",
		"SIGIL_v1_2.wad",
		"SIGIL.wad"
	};

	static const struct {
		const char *name;
		const char new_name[10];
	} sigil_lumps [] = {
		{"CREDIT",   "SIGCREDI"},
		{"HELP1",    "SIGHELP1"},
		{"TITLEPIC", "SIGTITLE"},
		{"DEHACKED", "SIG_DEH"},
		{"DEMO1",    "SIGDEMO1"},
		{"DEMO2",    "SIGDEMO2"},
		{"DEMO3",    "SIGDEMO3"},
		{"DEMO4",    "SIGDEMO4"},
		{"D_INTER",  "D_SIGINT"},
		{"D_INTRO",  "D_SIGTIT"},
	};

	// [crispy] don't load SIGIL.WAD if another PWAD already provides E5M1
	auto i = W_CheckNumForName("E5M1");
	if (i != -1)
	{
		// [crispy] indicate that SIGIL_*.WAD is already loaded as a PWAD
		if (!strncasecmp(W_WadNameForLump(lumpinfo[i]), "SIGIL", 5))
		{
			crispy->havesigil = (char *)-1;
		}
		return false;
	}

	// [crispy] don't load SIGIL.WAD if SIGIL_COMPAT.WAD is already loaded
	i = W_CheckNumForName("E3M1");
	if (i != -1 && !strncasecmp(W_WadNameForLump(lumpinfo[i]), "SIGIL_COMPAT", 12))
	{
		return false;
	}

	if (pwadtexture)
	{
		return false;
	}

	// [crispy] load SIGIL.WAD
	for (int i = 0; i < static_cast<int>(arrlen(sigil_wads)); i++)
	{
		crispy->havesigil = M_StringJoin(iwaddir, DIR_SEPARATOR_S, sigil_wads[i], NULL);

		if (M_FileExists(crispy->havesigil))
		{
			break;
		}

		free(crispy->havesigil);
		crispy->havesigil = D_FindWADByName(sigil_wads[i]);

		if (crispy->havesigil)
		{
			break;
		}
	}

	if (crispy->havesigil == NULL)
	{
		return false;
	}

	printf(" [Sigil] adding %s\n", crispy->havesigil);
	W_AddFile(crispy->havesigil);
	sigil_basename = M_BaseName(crispy->havesigil);

	auto sigil_shreds = std::string(iwaddir) + DIR_SEPARATOR_S + "SIGIL_SHREDS.WAD";

	// [crispy] load SIGIL_SHREDS.WAD
	if (!M_FileExists(sigil_shreds.c_str()))
	{
		sigil_shreds = str_ptr(D_FindWADByName("SIGIL_SHREDS.WAD")).get();
	}

	if (!sigil_shreds.empty())
	{
		printf(" [Sigil Shreds] adding %s\n", sigil_shreds.c_str());
		W_AddFile(sigil_shreds.c_str());
	}

	// [crispy] rename intrusive SIGIL_SHREDS.WAD music lumps out of the way
	for (int i = 0; i < static_cast<int>(arrlen(sigil_lumps)); i++)
	{
		// [crispy] skip non-music lumps
		if (strncasecmp(sigil_lumps[i].name, "D_", 2))
		{
			continue;
		}

		j = W_CheckNumForName(sigil_lumps[i].name);

		if (j != -1 && !strncasecmp(W_WadNameForLump(lumpinfo[j]), "SIGIL_SHREDS", 12))
		{
			memcpy(lumpinfo[j]->name, sigil_lumps[i].new_name, 8);
		}
	}

	// [crispy] rename intrusive SIGIL.WAD graphics, demos and music lumps out of the way
	for (int i = 0; i < static_cast<int>(arrlen(sigil_lumps)); i++)
	{
		j = W_CheckNumForName(sigil_lumps[i].name);

		if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), sigil_basename))
		{
			memcpy(lumpinfo[j]->name, sigil_lumps[i].new_name, 8);
		}
	}

	// [crispy] load WAD and DEH files from autoload directories
	if (!M_ParmExists("-noautoload"))
	{
		if (auto autoload_dir = str_ptr(M_GetAutoloadDir(sigil_basename, false)))
		{
			W_AutoLoadWADs(autoload_dir.get());
			DEH_AutoLoadPatches(autoload_dir.get());
		}
	}

	return true;
}

// [crispy] auto-load Sigil II
static boolean LoadSigil2Wad (const char *iwaddir, boolean pwadtexture)
{
    int j;
    const char *sigil2_basename;

    const char *const sigil2_wads[] = {
        "SIGIL_II_MP3_V1_0.WAD",
        "SIGIL_II_V1_0.WAD",
    };

    static const struct {
        const char *name;
        const char new_name[9];
    } sigil2_lumps [] = {
        {"CREDIT",   "SG2CREDI"},
        {"HELP1",    "SG2HELP1"},
        {"TITLEPIC", "SG2TITLE"},
        {"SIGILEND", "SGL2END"},
        {"DEHACKED", "SG2_DEH"},
        {"DEMO1",    "SG2DEMO1"},
        {"DEMO2",    "SG2DEMO2"},
        {"DEMO3",    "SG2DEMO3"},
        {"DEMO4",    "SG2DEMO4"},
        {"D_INTER",  "D_SG2INT"},
        {"D_INTRO",  "D_SG2TIT"},
    };

    // [crispy] don't load Sigil II if another PWAD already provides E6M1
    auto i = W_CheckNumForName("E6M1");
    if (i != -1)
    {
        // [crispy] indicate that SIGIL_II_*.WAD is already loaded as a PWAD
        if (!strncasecmp(W_WadNameForLump(lumpinfo[i]), "SIGIL_II", 8))
        {
            crispy->havesigil2 = (char *)-1;
        }

        return false;
    }

    // [crispy] don't load Sigil II if another PWAD already modifies the
    // texture files
    if (pwadtexture)
    {
        return false;
    }

    // [crispy] load Sigil II
    for (int i = 0; i < static_cast<int>(arrlen(sigil2_wads)); i++)
    {
        crispy->havesigil2 = M_StringJoin(iwaddir, DIR_SEPARATOR_S,
                                         sigil2_wads[i], NULL);

        if (M_FileExists(crispy->havesigil2))
        {
            break;
        }

        free(crispy->havesigil2);
        crispy->havesigil2 = D_FindWADByName(sigil2_wads[i]);

        if (crispy->havesigil2)
        {
            break;
        }
    }

    if (crispy->havesigil2 == NULL)
    {
        return false;
    }

    printf(" [Sigil II] adding %s\n", crispy->havesigil2);
    W_MergeFile(crispy->havesigil2);
    sigil2_basename = M_BaseName(crispy->havesigil2);

    // [crispy] rename intrusive Sigil II graphics, demos and music lumps out
    // of the way
    for (int i = 0; i < static_cast<int>(arrlen(sigil2_lumps)); i++)
    {
        j = W_CheckNumForName(sigil2_lumps[i].name);

        if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), sigil2_basename))
        {
            memcpy(lumpinfo[j]->name, sigil2_lumps[i].new_name, 8);
        }
    }

    // [crispy] load WAD and DEH files from autoload directories
    if (!M_ParmExists("-noautoload"))
    {
        if (auto autoload_dir = str_ptr(M_GetAutoloadDir(sigil2_basename, false)); autoload_dir)
        {
            W_AutoLoadWADs(autoload_dir.get());
            DEH_AutoLoadPatches(autoload_dir.get());
        }
    }

    return true;
}

// [crispy] auto-load Sigil and Sigil II if available
void D_LoadSigilWads (void)
{
    int j;
    boolean sigilloaded, sigil2loaded;
    boolean pwadtexture = false;

    const char *const texture_file[] = {
        "PNAMES",
        "TEXTURE1",
        "TEXTURE2",
    };

    // [crispy] don't load Sigils if another PWAD already modifies the texture
    // files
    for (int i = 0; i < static_cast<int>(arrlen(texture_file)); i++)
    {
        j = W_CheckNumForName(texture_file[i]);

        if (j != -1 && !W_IsIWADLump(lumpinfo[j]))
        {
            pwadtexture = true;
            break;
        }
    }

    auto iwaddir = str_ptr(M_DirName(iwadfile));
    sigilloaded = LoadSigilWad(iwaddir.get(), pwadtexture);
    sigil2loaded = LoadSigil2Wad(iwaddir.get(), pwadtexture);

    if (sigilloaded || sigil2loaded)
    {
        // [crispy] regenerate the hashtable
        W_GenerateHashTable();
    }

}

// [crispy] check if NERVE.WAD is already loaded as a PWAD
static boolean CheckNerveLoaded (void)
{
	int i, j;

	if ((i = W_GetNumForName("MAP01")) != -1 &&
	    (j = W_GetNumForName("MAP09")) != -1 &&
	    !strcasecmp(W_WadNameForLump(lumpinfo[i]), "NERVE.WAD") &&
	    !strcasecmp(W_WadNameForLump(lumpinfo[j]), "NERVE.WAD"))
	{
		gamemission = pack_nerve;

		// [crispy] if NERVE.WAD does not contain TITLEPIC, use INTERPIC instead
		i = W_GetNumForName("INTERPIC");
		j = W_GetNumForName("TITLEPIC");
		if (W_IsIWADLump(lumpinfo[i]) &&
		    strcasecmp(W_WadNameForLump(lumpinfo[j]), "NERVE.WAD"))
		{
			DEH_AddStringReplacement ("TITLEPIC", "INTERPIC");
		}

		return true;
	}

	return false;
}

// [crispy] auto-load NERVE.WAD if available
static void CheckLoadNerve (void)
{
	const char *nerve_basename;
	int i, j;

	static const struct {
		const char *name;
		std::string new_name;
	} nerve_lumps [] = {
		{"TITLEPIC", std::string("NERVEPIC")},
		{"INTERPIC", std::string("NERVEINT")},
	};

	// [crispy] don't load if another PWAD already provides MAP01
	i = W_CheckNumForName("MAP01");
	if (i != -1 && !W_IsIWADLump(lumpinfo[i]))
	{
		return;
	}

	if (strrchr(iwadfile, DIR_SEPARATOR) != NULL)
	{
		char *dir;
		dir = M_DirName(iwadfile);
		crispy->havenerve = M_StringJoin(dir, DIR_SEPARATOR_S, "NERVE.WAD", NULL);
	}
	else
	{
		crispy->havenerve = M_StringDuplicate("NERVE.WAD");
	}

	if (!M_FileExists(crispy->havenerve))
	{
		free(crispy->havenerve);
		crispy->havenerve = D_FindWADByName("NERVE.WAD");
	}

	if (crispy->havenerve == NULL)
	{
		return;
	}

	printf(" [No Rest for the Living] adding %s\n", crispy->havenerve);
	W_AddFile(crispy->havenerve);
	nerve_basename = M_BaseName(crispy->havenerve);

	// [crispy] add indicators to level and level name patch lump names
	for (i = 0; i < 9; i++)
	{
		char lumpname[9];

		M_snprintf (lumpname, 9, "CWILV%2.2d", i);
		j = W_GetNumForName(lumpname);
		lumpinfo[j]->name[0] = 'N';

		M_snprintf (lumpname, 9, "MAP%02d", i + 1);
		j = W_GetNumForName(lumpname);
		strcat(lumpinfo[j]->name, "N");
	}

	// [crispy] if NERVE.WAD contains TITLEPIC and INTERPIC, rename them
	for (int i = 0; i < static_cast<int>(arrlen(nerve_lumps)); i++)
	{
		j = W_CheckNumForName(nerve_lumps[i].name);

		if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), "NERVE.WAD"))
		{
			memcpy(lumpinfo[j]->name, nerve_lumps[i].new_name.c_str(), 8);
		}
	}

	// [crispy] load WAD and DEH files from autoload directories
	if (!M_ParmExists("-noautoload"))
	{
		if (auto autoload_dir = str_ptr(M_GetAutoloadDir(nerve_basename, false)); autoload_dir)
		{
			W_AutoLoadWADs(autoload_dir.get());
			DEH_AutoLoadPatches(autoload_dir.get());
		}
	}

	// [crispy] regenerate the hashtable
	W_GenerateHashTable();

	return;
}

void D_LoadNerveWad (void)
{
	// [crispy] check if NERVE.WAD is already loaded as a PWAD
	if (!CheckNerveLoaded())
	{
		// [crispy] else auto-load NERVE.WAD if available
		CheckLoadNerve();
	}
}

// [crispy] check if the single MASTERLEVELS.WAD is already loaded as a PWAD
static boolean CheckMasterlevelsLoaded (void)
{
	int i, j;

	if ((i = W_GetNumForName("MAP01")) != -1 &&
	    (j = W_GetNumForName("MAP21")) != -1 &&
	    !strcasecmp(W_WadNameForLump(lumpinfo[i]), "MASTERLEVELS.WAD") &&
	    !strcasecmp(W_WadNameForLump(lumpinfo[j]), "MASTERLEVELS.WAD"))
	{
		gamemission = pack_master;

		return true;
	}

	return false;
}

// [crispy] auto-load the single MASTERLEVELS.WAD if available
static boolean CheckLoadMasterlevels (void)
{
	const char *master_basename;
	int i, j;

	// [crispy] don't load if another PWAD already provides MAP01
	i = W_CheckNumForName("MAP01");
	if (i != -1 && !W_IsIWADLump(lumpinfo[i]))
	{
		return false;
	}

	if (strrchr(iwadfile, DIR_SEPARATOR) != NULL)
	{
		auto dir = str_ptr(M_DirName(iwadfile));
		crispy->havemaster = M_StringJoin(dir.get(), DIR_SEPARATOR_S, "MASTERLEVELS.WAD", NULL);
	}
	else
	{
		crispy->havemaster = M_StringDuplicate("MASTERLEVELS.WAD");
	}

	if (!M_FileExists(crispy->havemaster))
	{
		free(crispy->havemaster);
		crispy->havemaster = D_FindWADByName("MASTERLEVELS.WAD");
	}

	if (crispy->havemaster == NULL)
	{
		return false;
	}

	printf(" [The Master Levels] adding %s\n", crispy->havemaster);
	W_AddFile(crispy->havemaster);
	master_basename = M_BaseName(crispy->havemaster);

	// [crispy] add indicators to level and level name patch lump names
	for (i = 0; i < 21; i++)
	{
		char lumpname[9];

		M_snprintf(lumpname, 9, "CWILV%2.2d", i);
		j = W_GetNumForName(lumpname);
		if (!strcasecmp(W_WadNameForLump(lumpinfo[j]), "MASTERLEVELS.WAD"))
		{
			lumpinfo[j]->name[0] = 'M';
		}
		else
		{
			// [crispy] indicate this is not the complete MASTERLEVELS.WAD
			crispy->havemaster = (char *)-1;
		}


		M_snprintf(lumpname, 9, "MAP%02d", i + 1);
		j = W_GetNumForName(lumpname);
		strcat(lumpinfo[j]->name, "M");
	}

	// [crispy] load WAD and DEH files from autoload directories
	if (!M_ParmExists("-noautoload"))
	{
		if (auto autoload_dir = str_ptr(M_GetAutoloadDir(master_basename, false)); autoload_dir)
		{
			W_AutoLoadWADs(autoload_dir.get());
			DEH_AutoLoadPatches(autoload_dir.get());
		}
	}

	// [crispy] regenerate the hashtable
	W_GenerateHashTable();

	return true;
}

// [crispy] check if the 20 individual separate Mater Levels PWADs are available

static struct {
	const char *wad_name;
	int pc_slot;
	int psn_slot;
	boolean custom_sky;
	char *file_path;
} masterlevels_wads [] = {
	{"ATTACK.WAD",    1,  1},
	{"CANYON.WAD",    1,  2},
	{"CATWALK.WAD",   1,  3},
	{"COMBINE.WAD",   1,  4, true},
	{"FISTULA.WAD",   1,  5},
	{"GARRISON.WAD",  1,  6},
	{"MANOR.WAD",     1,  7, true},
	{"PARADOX.WAD",   1,  8},
	{"SUBSPACE.WAD",  1,  9},
	{"SUBTERRA.WAD",  1, 10},
	{"TTRAP.WAD",     1, 11, true},
	{"VIRGIL.WAD",    3, 12, true},
	{"MINOS.WAD",     5, 13, true},
	{"BLOODSEA.WAD",  7, 14},
	{"MEPHISTO.WAD",  7, 15},
	{"NESSUS.WAD",    7, 16, true},
	{"GERYON.WAD",    8, 17, true},
	{"VESPERAS.WAD",  9, 18, true},
	{"BLACKTWR.WAD", 25, 19},
	{"TEETH.WAD",    31, 20},
	{NULL,           32, 21}, // [crispy] TEETH.WAD
};

static boolean CheckMasterlevelsAvailable (void)
{
	int i;

	// [crispy] don't load if another PWAD already provides MAP01
	i = W_CheckNumForName("MAP01");
	if (i != -1 && !W_IsIWADLump(lumpinfo[i]))
	{
		return false;
	}

	auto dir = str_ptr(M_DirName(iwadfile));

	for (i = 0; masterlevels_wads[i].wad_name; i++)
	{
		str_ptr havemaster;

		if (strrchr(iwadfile, DIR_SEPARATOR) != NULL)
		{
			havemaster = str_ptr(M_StringJoin(dir.get(), DIR_SEPARATOR_S, masterlevels_wads[i].wad_name, NULL));
		}
		else
		{
			havemaster = str_ptr(M_StringDuplicate(masterlevels_wads[i].wad_name));
		}

		if (!M_FileExists(havemaster.get()))
		{
			havemaster = str_ptr(D_FindWADByName(masterlevels_wads[i].wad_name));
		}

		if (!havemaster)
		{
			int j;

			for (j = 0; j < i; j++)
			{
				free(masterlevels_wads[i].file_path);
			}

			return false;
		}

		masterlevels_wads[i].file_path = havemaster.release();
	}

	return true;
}

// [crispy] auto-load the 20 individual separate Mater Levels PWADs as if the were the single MASTERLEVELS.WAD
static void LoadMasterlevelsWads (void)
{
	char lumpname[9];

	const char *const sky_lumps[] = {
		"RSKY1",
		"PNAMES",
		"TEXTURE1",
	};

	for (int i = 0; i < static_cast<int>(arrlen(masterlevels_wads)); i++)
	{
		// [crispy] add TEETH.WAD only once
		if (masterlevels_wads[i].wad_name)
		{
			printf(" [The Master Levels %02d] adding %s\n", i+1, masterlevels_wads[i].file_path);
			W_AddFile(masterlevels_wads[i].file_path);
			free(masterlevels_wads[i].file_path);

			// [crispy] rename lumps changing the SKY texture out of the way
			if (masterlevels_wads[i].custom_sky)
			{
				for (int j = 0; j < static_cast<int>(arrlen(sky_lumps)); j++)
				{
					int k;

					k = W_CheckNumForName(sky_lumps[j]);

					if (k != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[k]), masterlevels_wads[i].wad_name))
					{
						lumpinfo[k]->name[0] = 'M';
					}
				}
			}
		}

		M_snprintf(lumpname, 9, "MAP%02d", masterlevels_wads[i].pc_slot);
		auto j = W_GetNumForName(lumpname);
		lumpinfo[j]->name[3] = '0' + (masterlevels_wads[i].psn_slot / 10);
		lumpinfo[j]->name[4] = '0' + (masterlevels_wads[i].psn_slot % 10);
		lumpinfo[j]->name[5] = 'M';
	}

	// [crispy] indicate this is not the single MASTERLEVELS.WAD
	crispy->havemaster = (char *)-1;

	// [crispy] regenerate the hashtable
	W_GenerateHashTable();
	return;
}

void D_LoadMasterlevelsWad (void)
{
	// [crispy] check if the single MASTERLEVELS.WAD is already loaded as a PWAD
	if (!CheckMasterlevelsLoaded())
	{
		// [crispy] else auto-load the single MASTERLEVELS.WAD if available
		if (!CheckLoadMasterlevels())
		{
			// [crispy] else check if the 20 individual separate Mater Levels PWADs are available
			if (CheckMasterlevelsAvailable())
			{
				// [crispy] and auto-load them as if the were the single MASTERLEVELS.WAD
				LoadMasterlevelsWads();
			}
		}
	}
}
