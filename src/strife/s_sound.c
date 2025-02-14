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
// DESCRIPTION:  none
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "i_sound.h"
#include "i_system.h"

#include "deh_str.h"

#include "doomstat.h"
#include "doomtype.h"

#include "sounds.h"
#include "s_sound.h"

#include "m_misc.h"
#include "m_random.h"
#include "m_argv.h"

#include "p_local.h"
#include "w_wad.h"
#include "z_zone.h"

// when to clip out sounds
// Does not fit the large outdoor areas.

#define S_CLIPPING_DIST (1200 * FRACUNIT)

// Distance tp origin when sounds should be maxed out.
// This should relate to movement clipping resolution
// (see BLOCKMAP handling).
// In the source code release: (160*FRACUNIT).  Changed back to the
// Vanilla value of 200 (why was this changed?)

#define S_CLOSE_DIST (200 * FRACUNIT)

// The range over which sound attenuates

#define S_ATTENUATOR ((S_CLIPPING_DIST - S_CLOSE_DIST) >> FRACBITS)

// Stereo separation

#define S_STEREO_SWING (96 * FRACUNIT)
static int stereo_swing = S_STEREO_SWING; // [crispy]

#define NORM_PRIORITY 64
#define NORM_SEP 128

typedef struct
{
    // sound information (if null, channel avail.)
    sfxinfo_t *sfxinfo;

    // origin of sound
    mobj_t *origin;

    // handle of the sound being played
    int handle;

    int pitch;

} channel_t;

// The set of channels available

static channel_t *channels;
static degenmobj_t *sobjs; // [crispy] sound objects

// Maximum volume of a sound effect.
// Internal default is max out of 0-15.

int sfxVolume = 8;

// Maximum volume of music.

int musicVolume = 13;

// haleyjd 08/29/10: [STRIFE] New global variable
// Volume of voice channel.

int voiceVolume = 15;

// Internal volume level, ranging from 0-127

static int snd_SfxVolume;

// haleyjd 09/11/10: [STRIFE] Internal voice volume level

static int snd_VoiceVolume;

// Whether songs are mus_paused

static boolean mus_paused;

// Music currently being played

static musicinfo_t *mus_playing = NULL;

// Number of channels to use

int snd_channels = 8;

// haleyjd 09/11/10: [STRIFE] Handle of current voice channel.
// This has been implemented at a higher level than it was implemented
// in strife1.exe, as there it relied on a priority system which was
// implicit in the SFX_PlayPatch API of DMX. Here we'll just ignore
// the current voice channel when doing normal sound playing.

static int i_voicehandle = -1;

// haleyjd 09/11/10: [STRIFE] whether to play voices or not
int disable_voices = 0;

//
// Initializes sound stuff, including volume
// Sets channels, SFX and music volume,
//  allocates channel buffer, sets S_sfx lookup.
//
// haleyjd 09/11/10: [STRIFE] Added voice volume
//
void S_Init(int sfxVolume, int musicVolume, int voiceVolume)
{
    int i;

    I_SetOPLDriverVer(opl_doom_1_9);
    I_PrecacheSounds(S_sfx, NUMSFX);

    S_SetSfxVolume(sfxVolume);
    S_SetMusicVolume(musicVolume);
    S_SetVoiceVolume(voiceVolume);

    // Allocating the internal channels for mixing
    // (the maximum numer of sounds rendered
    // simultaneously) within zone memory.
    // [crispy] variable number of sound channels
    channels = I_Realloc(NULL, snd_channels*sizeof(channel_t));
    sobjs = I_Realloc(NULL, snd_channels*sizeof(degenmobj_t)); // [crispy] sound objects

    // Free all channels for use
    for (i=0 ; i<snd_channels ; i++)
    {
        channels[i].sfxinfo = 0;
    }

    // no sounds are playing, and they are not mus_paused
    mus_paused = 0;

    // Note that sounds have not been cached (yet).
    for (i=1 ; i<NUMSFX ; i++)
    {
        S_sfx[i].lumpnum = S_sfx[i].usefulness = -1;
    }

    I_AtExit(S_Shutdown, true);

    // [crispy] handle stereo separation for mono-sfx
    S_UpdateStereoSeparation();
}

void S_Shutdown(void)
{
    I_ShutdownSound();
    I_ShutdownMusic();
}

static void S_StopChannel(int cnum)
{
    int i;
    channel_t *c;

    c = &channels[cnum];

    // haleyjd: [STRIFE] If stopping the voice channel, set i_voicehandle to -1
    if (cnum == i_voicehandle)
        i_voicehandle = -1;

    if (c->sfxinfo)
    {
        // stop the sound playing

        if (I_SoundIsPlaying(c->handle))
        {
            I_StopSound(c->handle);
        }

        // check to see if other channels are playing the sound

        for (i=0; i<snd_channels; i++)
        {
            if (cnum != i && c->sfxinfo == channels[i].sfxinfo)
            {
                break;
            }
        }

        // degrade usefulness of sound data

        c->sfxinfo->usefulness--;
        c->sfxinfo = NULL;
    }
}

//
// Per level startup code.
// Kills playing sounds at start of level,
//  determines music if any, changes music.
//
// haleyjd 08/31/10: [STRIFE]
// * Removed DOOM music handling and replaced with Strife code.
//
void S_Start(void)
{
    int cnum;
    int mnum;

    // kill all playing sounds at start of level
    //  (trust me - a good idea)
    for (cnum=0 ; cnum<snd_channels ; cnum++)
    {
        if (channels[cnum].sfxinfo)
        {
            S_StopChannel(cnum);
        }
    }

    // start new music for the level
    mus_paused = 0;

    // [STRIFE] Some interesting math here ;)
    if(gamemap <= 31)
        mnum = 1;
    else
        mnum = -30;

    S_ChangeMusic(gamemap + mnum, true);
}

void S_StopSound(mobj_t *origin)
{
    int cnum;

    for (cnum=0 ; cnum<snd_channels ; cnum++)
    {
        // haleyjd: do not stop voice here.
        if(cnum == i_voicehandle)
            continue;

        if (channels[cnum].sfxinfo && channels[cnum].origin == origin)
        {
            S_StopChannel(cnum);
            break;
        }
    }
}

// [crispy] removed map objects may finish their sounds
// When map objects are removed from the map by P_RemoveMobj(), instead of
// stopping their sounds, their coordinates are transfered to "sound objects"
// so stereo positioning and distance calculations continue to work even after
// the corresponding map object has already disappeared.
// Thanks to jeff-d and kb1 for discussing this feature and the former for the
// original implementation idea: https://www.doomworld.com/vb/post/1585325
void S_UnlinkSound(mobj_t *origin)
{
    int cnum;

    if (origin)
    {
        for (cnum=0 ; cnum<snd_channels ; cnum++)
        {
            if (channels[cnum].sfxinfo && channels[cnum].origin == origin)
            {
                degenmobj_t *const sobj = &sobjs[cnum];
                sobj->x = origin->x;
                sobj->y = origin->y;
                sobj->z = origin->z;
                channels[cnum].origin = (mobj_t *) sobj;
                break;
            }
        }
    }
}

//
// S_GetChannel :
//   If none available, return -1.  Otherwise channel #.
//
// haleyjd 09/11/10: [STRIFE] Added an "isvoice" parameter for supporting
// voice playing.
//
static int S_GetChannel(mobj_t *origin, sfxinfo_t *sfxinfo, boolean isvoice)
{
    // channel number to use
    int                cnum;

    channel_t*        c;

    // Find an open channel
    for (cnum=0 ; cnum<snd_channels ; cnum++)
    {
        if (!channels[cnum].sfxinfo)
        {
            break;
        }
        else if (origin && channels[cnum].origin == origin &&
                 (isvoice || cnum != i_voicehandle)) // haleyjd
        {
            // haleyjd 20150220: [STRIFE] missing sound channel priority check
            // Is a higher priority sound by same origin already playing?
            if(!isvoice && sfxinfo->priority > channels[cnum].sfxinfo->priority)
                return -1;

            S_StopChannel(cnum);
            break;
        }
    }

    // None available
    if (cnum == snd_channels)
    {
        // Look for lower priority
        for (cnum=0 ; cnum<snd_channels ; cnum++)
        {
            if (channels[cnum].sfxinfo->priority >= sfxinfo->priority)
            {
                // haleyjd 09/11/10: [STRIFE] voice has absolute priority
                if (isvoice || cnum != i_voicehandle)
                    break;
            }
        }

        if (cnum == snd_channels)
        {
            // FUCK!  No lower priority.  Sorry, Charlie.
            return -1;
        }
        else
        {
            // Otherwise, kick out lower priority.
            S_StopChannel(cnum);
        }
    }

    c = &channels[cnum];

    // channel is decided to be cnum.
    c->sfxinfo = sfxinfo;
    c->origin = origin;

    return cnum;
}

//
// Changes volume and stereo-separation variables
//  from the norm of a sound effect to be played.
// If the sound is not audible, returns a 0.
// Otherwise, modifies parameters and returns 1.
//
// [STRIFE]
// haleyjd 20110220: changed to eliminate the gamemap == 8 hack that was
// left over from Doom 1's original boss levels. Kind of amazing that Rogue
// was able to catch the smallest things like that.
//
static int S_AdjustSoundParams(mobj_t *listener, mobj_t *source,
                               int *vol, int *sep)
{
    fixed_t        approx_dist;
    fixed_t        adx;
    fixed_t        ady;
    angle_t        angle;

    // calculate the distance to sound origin
    //  and clip it if necessary
    adx = abs(listener->x - source->x);
    ady = abs(listener->y - source->y);

    // From _GG1_ p.428. Appox. eucledian distance fast.
    approx_dist = adx + ady - ((adx < ady ? adx : ady)>>1);

    // [STRIFE] removed gamemap == 8 hack
    if (approx_dist > S_CLIPPING_DIST)
    {
        return 0;
    }

    // angle of source to listener
    angle = R_PointToAngle2(listener->x,
                            listener->y,
                            source->x,
                            source->y);

    if (angle > listener->angle)
    {
        angle = angle - listener->angle;
    }
    else
    {
        angle = angle + (0xffffffff - listener->angle);
    }

    angle >>= ANGLETOFINESHIFT;

    // stereo separation
    // [crispy] mono sound option
    *sep = 128 - (FixedMul(stereo_swing, finesine[angle]) >> FRACBITS);

    // volume calculation
    // [STRIFE] Removed gamemap == 8 hack
    if (approx_dist < S_CLOSE_DIST)
    {
        *vol = snd_SfxVolume;
    }
    else
    {
        // distance effect
        *vol = (snd_SfxVolume
                * ((S_CLIPPING_DIST - approx_dist)>>FRACBITS))
            / S_ATTENUATOR;
    }

    return (*vol > 0);
}

void S_StartSound(void *origin_p, int sfx_id)
{
    sfxinfo_t *sfx;
    mobj_t *origin;
    int rc;
    int sep;
    int pitch;
    int cnum;
    int volume;

    origin = (mobj_t *) origin_p;
    volume = snd_SfxVolume;

    // check for bogus sound #
    if (sfx_id < 1 || sfx_id > NUMSFX)
    {
        // [STRIFE]: BUG - Note: vanilla had some extremely buggy and dangerous
        // code here that tried to print the sprite name of the object playing
        // the bad sound. Because it invokes multiple undefined behaviors and
        // is of basically no consequence, it has deliberately not been ported.
        I_Error("Bad sfx #: %d", sfx_id);
    }

    sfx = &S_sfx[sfx_id];

    // Initialize sound parameters
    if (sfx->link)
    {
        volume += sfx->volume;

        if (volume < 1)
        {
            return;
        }

        if (volume > snd_SfxVolume)
        {
            volume = snd_SfxVolume;
        }
    }


    // Check to see if it is audible,
    //  and if not, modify the params
    if (origin && origin != players[consoleplayer].mo &&
        origin != players[displayplayer].so) // [crispy] weapon sound source
    {
        rc = S_AdjustSoundParams(players[consoleplayer].mo,
                                 origin,
                                 &volume,
                                 &sep);

        if (origin->x == players[consoleplayer].mo->x
         && origin->y == players[consoleplayer].mo->y)
        {
            sep = NORM_SEP;
        }

        if (!rc)
        {
            return;
        }
    }
    else
    {
        sep = NORM_SEP;
    }
    pitch = NORM_PITCH;

    // kill old sound [STRIFE] - nope!
    //S_StopSound(origin);

    // try to find a channel
    cnum = S_GetChannel(origin, sfx, false); // haleyjd: not a voice.

    if (cnum < 0)
    {
        return;
    }

    // increase the usefulness
    if (sfx->usefulness++ < 0)
    {
        sfx->usefulness = 1;
    }

    if (sfx->lumpnum < 0)
    {
        sfx->lumpnum = I_GetSfxLumpNum(sfx);
    }

    channels[cnum].handle = I_StartSound(sfx, cnum, volume, sep, pitch);
}

// [crispy]
void S_StartSoundOnce (void *origin_p, int sfx_id)
{
    int cnum;
    const sfxinfo_t *const sfx = &S_sfx[sfx_id];

    for (cnum = 0; cnum < snd_channels; cnum++)
    {
        if (channels[cnum].sfxinfo == sfx &&
            channels[cnum].origin == origin_p)
        {
            return;
        }
    }

    S_StartSound(origin_p, sfx_id);
}


// haleyjd 09/11/10: [STRIFE]
// None of this was necessary in the vanilla EXE but Choco's low-level code
// won't play nice with a temporary sfxinfo because it insists that the
// "driver_data" member remain valid from the last time the sound was used,
// even if it has already stopped playing. Thanks to this cuteness I get
// to maintain a dynamic cache of sfxinfo objects!

typedef struct voiceinfo_s
{
    sfxinfo_t sfx;
    struct voiceinfo_s *next; // next on hash chain
} voiceinfo_t;

#define NUMVOICECHAINS 257

//
// Ripped from Eternity.
//
static unsigned int S_voiceHash(const char *str)
{
   const char *c = str;
   unsigned int h = 0;

   if(!str)
      I_Error("S_voiceHash: cannot hash NULL string!\n");

   // note: this needs to be case insensitive for lump names
   while(*c)
   {
      h = 5 * h + toupper(*c);
      ++c;
   }

   return h;
}

static voiceinfo_t *voices[NUMVOICECHAINS];

//
// S_getVoice
//
// Gets an entry from the voice table, if it exists. If it does not, one will be
// created.
//
static voiceinfo_t *S_getVoice(const char *name, int lumpnum)
{
    voiceinfo_t *voice;
    unsigned int hashkey = S_voiceHash(name) % NUMVOICECHAINS;

    voice = voices[hashkey];

    while(voice && strcasecmp(voice->sfx.name, name))
        voice = voice->next;

    if(!voice)
    {
        voice = calloc(1, sizeof(voiceinfo_t));

        M_StringCopy(voice->sfx.name, name, sizeof(voice->sfx.name));
        voice->sfx.priority = INT_MIN; // make highest possible priority
        voice->sfx.pitch = -1;
        voice->sfx.volume = -1;
        voice->sfx.numchannels = -1;
        voice->sfx.usefulness = -1;
        voice->sfx.lumpnum = lumpnum;

        // throw it onto the table.
        voice->next = voices[hashkey];
        voices[hashkey] = voice;
    }

    return voice;
}

//
// I_StartVoice
//
// haleyjd 09/11/10: [STRIFE] New function
// Note this was in i_sound.c in Strife itself, but relied on DMX-specific
// features to ensure voice channels had absolute priority. Here we must
// populate a fake sfxinfo_t and send the sound through some of the normal
// routines. But in the end, it still works the same.
//
void I_StartVoice(const char *lumpname)
{
    int lumpnum;
    voiceinfo_t *voice; // choco-specific
    char lumpnamedup[9];

    // no voices in deathmatch mode.
    if(netgame)
        return;

    // STRIFE-TODO: checks if snd_SfxDevice == 83
    // This is probably turning off voice if using PC speaker...

    // user has disabled voices?
    if(disable_voices)
        return;

    // have a voice playing already? stop it.
    if(i_voicehandle >= 0)
        S_StopChannel(i_voicehandle);

    // Vanilla STRIFE appears to have stopped any current voice without
    // starting a new one if NULL was passed in here, though I cannot
    // find an explicit check for NULL in the assembly. Either way, it
    // didn't crash, so do a check now:
    if(lumpname == NULL)
        return;

    // Because of constness problems...
    M_StringCopy(lumpnamedup, lumpname, sizeof(lumpnamedup));

    if((lumpnum = W_CheckNumForName(lumpnamedup)) != -1)
    {
        // haleyjd: Choco-specific: get a voice structure
        voice = S_getVoice(lumpnamedup, lumpnum);

        // get a channel for the voice
        i_voicehandle = S_GetChannel(NULL, &voice->sfx, true);

        channels[i_voicehandle].handle
            = I_StartSound(&voice->sfx, i_voicehandle, snd_VoiceVolume, NORM_SEP, NORM_PITCH);
    }
}

//
// Stop and resume music, during game PAUSE.
//

void S_PauseSound(void)
{
    if (mus_playing && !mus_paused)
    {
        I_PauseSong();
        mus_paused = true;
    }
}

void S_ResumeSound(void)
{
    if (mus_playing && mus_paused)
    {
        I_ResumeSong();
        mus_paused = false;
    }
}

//
// Updates music & sounds
//

void S_UpdateSounds(mobj_t *listener)
{
    int                audible;
    int                cnum;
    int                volume;
    int                sep;
    sfxinfo_t*        sfx;
    channel_t*        c;

    I_UpdateSound();

    for (cnum=0; cnum<snd_channels; cnum++)
    {
        c = &channels[cnum];
        sfx = c->sfxinfo;

        if (c->sfxinfo)
        {
            if (I_SoundIsPlaying(c->handle))
            {
                // initialize parameters
                volume = snd_SfxVolume;
                sep = NORM_SEP;

                if (sfx->link)
                {
                    volume += sfx->volume;
                    if (volume < 1)
                    {
                        S_StopChannel(cnum);
                        continue;
                    }
                    else if (volume > snd_SfxVolume)
                    {
                        volume = snd_SfxVolume;
                    }
                }

                // check non-local sounds for distance clipping
                //  or modify their params
                if (c->origin && listener != c->origin &&
                    c->origin != players[displayplayer].so) // [crispy] weapon sound source
                {
                    audible = S_AdjustSoundParams(listener,
                                                  c->origin,
                                                  &volume,
                                                  &sep);

                    if (!audible)
                    {
                        S_StopChannel(cnum);
                    }
                    else
                    {
                        I_UpdateSoundParams(c->handle, volume, sep);
                    }
                }
            }
            else
            {
                // if channel is allocated but sound has stopped,
                //  free it
                S_StopChannel(cnum);
            }
        }
    }
}

void S_SetMusicVolume(int volume)
{
    if (volume < 0 || volume > 127)
    {
        I_Error("Attempt to set music volume at %d",
                volume);
    }

    I_SetMusicVolume(volume);
}

void S_SetSfxVolume(int volume)
{
    if (volume < 0 || volume > 127)
    {
        I_Error("Attempt to set sfx volume at %d", volume);
    }

    snd_SfxVolume = volume;
}

//
// S_SetVoiceVolume
//
// haleyjd 09/11/10: [STRIFE]
// Set the internal voice volume level.
//
void S_SetVoiceVolume(int volume)
{
    if (volume < 0 || volume > 127)
    {
        I_Error("Attempt to set voice volume at %d", volume);
    }

    snd_VoiceVolume = volume;
}

//
// Starts some music with the music id found in sounds.h.
//

void S_StartMusic(int m_id)
{
    S_ChangeMusic(m_id, false);
}

void S_ChangeMusic(int musicnum, int looping)
{
    musicinfo_t *music = NULL;
    char namebuf[9];
    void *handle;

    if (musicnum <= mus_None || musicnum >= NUMMUSIC)
    {
        I_Error("Bad music number %d", musicnum);
    }
    else
    {
        music = &S_music[musicnum];
    }

    if (mus_playing == music)
    {
        return;
    }

    // shutdown old music
    S_StopMusic();

    // get lumpnum if neccessary
    if (!music->lumpnum)
    {
        M_snprintf(namebuf, sizeof(namebuf), "d_%s", DEH_String(music->name));
        music->lumpnum = W_GetNumForName(namebuf);
    }

    music->data = W_CacheLumpNum(music->lumpnum, PU_STATIC);

    handle = I_RegisterSong(music->data, W_LumpLength(music->lumpnum));
    music->handle = handle;
    I_PlaySong(handle, looping);

    mus_playing = music;
}

boolean S_MusicPlaying(void)
{
    return I_MusicIsPlaying();
}

void S_StopMusic(void)
{
    if (mus_playing)
    {
        if (mus_paused)
        {
            I_ResumeSong();
        }

        I_StopSong();
        I_UnRegisterSong(mus_playing->handle);
        W_ReleaseLumpNum(mus_playing->lumpnum);
        mus_playing->data = NULL;
        mus_playing = NULL;
    }
}

// [crispy] variable number of sound channels
void S_UpdateSndChannels (int choice)
{
    int i;

    for (i = 0; i < snd_channels; i++)
    {
        if (channels[i].sfxinfo)
            S_StopChannel(i);
    }

    if (choice)
        snd_channels <<= 1;
    else
        snd_channels >>= 1;

    if (snd_channels > 32)
        snd_channels = 8;
    else if (snd_channels < 8)
        snd_channels = 32;

    channels = I_Realloc(channels, snd_channels * sizeof(channel_t));
    sobjs = I_Realloc(sobjs, snd_channels * sizeof(degenmobj_t)); // [crispy] sound objects

    for (i = 0; i < snd_channels; i++)
    {
        channels[i].sfxinfo = 0;
    }
}

// [crispy] play sound effects in stereo or mono
void S_UpdateStereoSeparation (void)
{
    if (crispy->soundmono)
        stereo_swing = 0;
    else
        stereo_swing = S_STEREO_SWING;
}

