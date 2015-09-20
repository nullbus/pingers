#include "Jukebox.h"
#include <Windows.h>
#include <tchar.h>
#include "Song.h"
#include "bms/BmsParser.h"

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

		//파일정보를 저장할 변수
		::WIN32_FIND_DATAA c_file;
		HANDLE hFile;

		clearSongs();

		char jukeboxDir[0xFF];
		strncpy(jukeboxDir, jukeboxPath, strlen(jukeboxPath)+1);
		strcat(jukeboxDir, "*.*");
		
		/*모든 파일정보를 찾는다.*/
		if( (hFile = ::FindFirstFileA( jukeboxDir, &c_file )) == INVALID_HANDLE_VALUE )
		{
			MessageBox(NULL, _T("Cannot open directory!"), 0, 0);
			return false;
		}

		while( FindNextFileA( hFile, &c_file ) )
		{
			if(c_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				findSongsAtFolder(c_file);
		}

		FindClose( hFile );

		if( !songs.size() )
		{
			MessageBox(0, _T("노래가 하나도 없네요"), _T("에러"), MB_OK);
			return false;
		}

		mLoaded = true;
		return true;
	}

	void Jukebox::findSongsAtFolder(WIN32_FIND_DATAA& folder)
	{
		if(strcmp(folder.cFileName, ".") == 0 || strcmp(folder.cFileName, "..") == 0)
			return;
		if(!(folder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return;

		if(strcmp(folder.cFileName, ".")==0 || strcmp(folder.cFileName, "..")==0)
			return;


		//파일정보를 저장할 변수
		WIN32_FIND_DATAA c_file;
	
		//식별자
		HANDLE hFile;

		BmsParser parser;
		std::string strPath = jukeboxPath;
		std::string postfix[] = {"*.bms", ".bme"};
		//일단은 .bms, .bme만 지원

		for(int i=0; i<sizeof(postfix)/sizeof(postfix[0]); i++)
		{
			hFile = FindFirstFileA((strPath + folder.cFileName + "/" + postfix[i]).c_str() , &c_file);
			if( hFile == INVALID_HANDLE_VALUE )
			{
				continue;
			}
			else
			{
				std::string path = strPath + folder.cFileName + "/" + c_file.cFileName;
				SongInfo *song = new SongInfo;
				parser.parseHeader(path.c_str(), *song);
				songs.push_back(song);

				/* Find the rest of the .c files */
				while( FindNextFileA( hFile, &c_file ) )
				{
					path = strPath + folder.cFileName + "/" + c_file.cFileName;
					song = new SongInfo;
					parser.parseHeader(path.c_str(), *song);
					songs.push_back(song);
				}
		
				FindClose( hFile );
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