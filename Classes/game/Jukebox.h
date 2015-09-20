/*
 *	Jukebox
 *	Song�� �����ϴ� Ŭ����.
 */

#ifndef __JUKEBOX_H__
#define __JUKEBOX_H__

#include <vector>
#include <string>
#include <Windows.h>


namespace BMS
{
	class SongInfo;

	class Jukebox
	{
	public:
		static Jukebox* getInstance();
	public:

		bool loadJukebox();
		SongInfo* getSong(int index);
		int getSongSize() { return songs.size(); }

		~Jukebox();

	protected:
		Jukebox();

		void clearSongs();
		void findSongsAtFolder(WIN32_FIND_DATAA&);

	protected:
		static Jukebox *s_instance;

	protected:
		std::vector<SongInfo*> songs;
		bool mLoaded;
	};
}

#endif // #ifdef __JUKEBOX_H__