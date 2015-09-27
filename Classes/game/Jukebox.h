/*
 *	Jukebox
 *	Song을 관리하는 클래스.
 */

#ifndef __JUKEBOX_H__
#define __JUKEBOX_H__

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

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
		void findSongsAtFolder(const boost::filesystem::path& dir);

	protected:
		static Jukebox *s_instance;

	protected:
		std::vector<SongInfo*> songs;
		bool mLoaded;
	};
}

#endif // #ifdef __JUKEBOX_H__
