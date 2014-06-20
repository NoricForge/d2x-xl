/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

#ifndef _SONGS_H
#define _SONGS_H

#include "maths.h"
#include "carray.h"
#include "cfile.h"

#define MAX_NUM_SONGS         100

#define SONG_TITLE            0
#define SONG_BRIEFING         1
#define SONG_ENDLEVEL         2
#define SONG_ENDGAME          3
#define SONG_CREDITS          4
#define SONG_FIRST_LEVEL_SONG 5
#define SONG_INTER				2

extern int Num_songs, nD1SongNum, nD2SongNum;   //how many MIDI songs

//------------------------------------------------------------------------------

class CRedbook {
	private:
		int	m_bForceRegister;
		int	m_bEnabled;
		int	m_bPlaying;
		fix	m_xLastCheck;

	public:
		CRedbook () { Init (); }
		~CRedbook () { Destroy (); }
		void Init (void);
		void Destroy (void);
		void Register (void);
		void SetVolume (int volume);
		int PlayTrack (int nTrack, int bKeepPlaying);
		void CheckRepeat (void);
		void ReInit (void);
		void Stop (void);
		int HaveD2CD (void);
		inline void ForceRegister (void) { m_bForceRegister = 1; }
		inline int Enabled (void) { return m_bEnabled; }
		inline void Enable (int bEnabled) { m_bEnabled = bEnabled; }
		inline int Playing (void) { return m_bPlaying; }
		inline void SetPlaying (int bPlaying) { m_bPlaying = bPlaying; }
};

extern CRedbook redbook;

//------------------------------------------------------------------------------

class CPlaylist {
	public:
		CArray<char*>		levelSongs [2];
		CArray<int>			songIndex [2];

	public:
		int SongCount (int bSecret);
		void ShuffleSongs (void);
		void SortSongs (void);
		void AlignSongs (void);
		int SongIndex (int nSong, int bSecret);
		int LoadPlaylist (char* pszFolder, char *pszPlaylist = "playlist.txt");
		void DestroyPlaylist (int* nSongs = NULL);
		inline char* LevelSong (int nSong, int bSecret) { return levelSongs [bSecret][SongIndex (bSecret, nSong)]; }
		int PlayLevelSong (int nSong, int bSecret, int bD1 = 0);

	private:
		void Shuffle (int bSecret);
		void Sort (int bSecret);
	};

class CSongData {
	public:
		char					filename [16];
		char					melodicBankFile [16];
		char					drumBankFile [16];
	};

class CSongInfo {
	public:
		CSongData			data [MAX_NUM_SONGS];
		int					songIndex [2][MAX_NUM_SONGS];
		int					bInitialized;
		int					bPlaying;
		int					nTotalSongs;
		int					nSongs [2];
		int					nFirstLevelSong [2];
		int					nLevelSongs [2];
		int					nCurrent;
		int					nLevel;
		int					nD1EndLevelSong;
		time_t				tStart;
		time_t				tSlowDown;
		time_t				tPos;
		char					szIntroSong [FILENAME_LEN];
		char					szBriefingSong [FILENAME_LEN];
		char					szCreditsSong [FILENAME_LEN];
		char					szMenuSong [FILENAME_LEN];

	public:
		inline int SongIndex (int nSong, int bD1) { return songIndex [bD1][nSong % nLevelSongs [bD1]]; }
	};

class CSongManager {
	private:
		CSongInfo	m_info;
		CPlaylist	m_descent [2];
		CPlaylist	m_user;
		CPlaylist	m_mod;

	public:
		CSongManager () { Init (); }
		~CSongManager () { Destroy (); }
		void Init (void);
		void Destroy (void);
		void Setup (void);
		void CheckRepeat (void);
		void StopAll (void);
		int PlayCustomSong (char* pszFolder, char* pszSong, int bLoop);
		void Play (int nSong, int repeat);
		int PlayCustomLevelSong (char* pszFolder, int nLevel, int nSong);
		void PlayLevelSong (int nLevel, int bFromHog, bool bWaitForThread = true);
		void PlayCurrent (int repeat = 0);
		void Prev (void);
		void Next (void);
		void ShuffleSongs (void);
		void SortSongs (void);
		void AlignSongs (void);
		int LoadDescentPlaylists (void);
		int LoadUserPlaylist (char *pszPlaylist);
		int LoadModPlaylist (void);
		inline void DestroyPlaylist (int* nSongs = NULL) {
			m_mod.DestroyPlaylist (nSongs);
			}
		inline void DestroyPlaylists (void) {
			m_mod.DestroyPlaylist ();
			}
		inline int Current (void) { return m_info.nCurrent; }
		inline int Playing (void) { return m_info.bPlaying; }
		inline void SetPlaying (int bPlaying) { m_info.bPlaying = bPlaying; }
		inline time_t Pos (void) { return m_info.tPos; }
		inline time_t Start (void) { return m_info.tStart; }
		inline time_t SlowDown (void) { return m_info.tSlowDown; }
		inline void SetPos (time_t t) { m_info.tPos = t; }
		inline void SetStart (time_t t) { m_info.tStart = t; }
		inline void SetSlowDown (time_t t) { m_info.tSlowDown = t; }
		inline char* IntroSong (void) { return m_info.szIntroSong; }
		inline char* BriefingSong (void) { return m_info.szBriefingSong; }
		inline char* CreditsSong (void) { return m_info.szCreditsSong; }
		inline char* MenuSong (void) { return m_info.szMenuSong; }
		inline int TotalCount (void) { return m_info.nTotalSongs; }
		inline int Count (uint i) { return m_info.nSongs [i]; }
		inline CSongData& SongData (uint i = 0) { return m_info.data [i]; }
	};

extern CSongManager songManager;

#endif /* _SONGS_H */
