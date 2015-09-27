#include "Jukebox.h"
#include "Song.h"
#include "bms/BmsParser.h"
#include "util/logutil.h"

using namespace boost::filesystem;

#pragma warning(disable:4996)
#pragma warning(disable:4018)

namespace BMS
{
	const char *jukeboxPath = "jukebox/";

	Jukebox* Jukebox::s_instance = new Jukebox();

	Jukebox* Jukebox::getInstance()
	{
		return s_instance;
	}


	Jukebox::Jukebox() : mLoaded(false)
	{

	}


	Jukebox::~Jukebox()
	{
		clearSongs();
	}


	bool Jukebox::loadJukebox()
	{
		if(mLoaded)
			return true;

		clearSongs();

		path root(jukeboxPath);
		if (!is_directory(root))
		{
			Log::i("cannot open directory %s!", jukeboxPath);
		}

		for (directory_entry& dir : directory_iterator(root))
		{
			if (is_directory(dir.path()))
			{
				findSongsAtFolder(dir.path());
			}
		}

		if( !songs.size() )
		{
			Log::i("no songs!");
			return false;
		}

		mLoaded = true;
		return true;
	}

	void Jukebox::findSongsAtFolder(const path& dir)
	{
		//if(strcmp(folder.cFileName, ".") == 0 || strcmp(folder.cFileName, "..") == 0)
		//	return;
		if(!is_directory(dir))
			return;

		BmsParser parser;
		std::string strPath = jukeboxPath;

		//일단은 .bms, .bme만 지원
		std::string postfix[] = {".bms", ".bme"};

		for (directory_entry& file : directory_iterator(dir))
		{
			if (!is_regular_file(file.path()))
				continue;

			for (const std::string& ext : postfix)
			{
				if (file.path().extension() != ext)
					continue;

				SongInfo* song = new SongInfo;
				parser.parseHeader(file.path().string().c_str(), *song);
				songs.push_back(song);
				break;
			}

		}
	}

	void Jukebox::clearSongs()
	{
		for(unsigned i=0; i<songs.size(); i++)
			delete songs[i];

		songs.clear();
	}

	SongInfo* Jukebox::getSong(int index)
	{
		if( !songs.size() )
			return NULL;

		if(index < 0 || index >= songs.size())
			return NULL;

		return songs.at(index);
	}
}