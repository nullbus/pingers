#pragma once

#include <windows.h>
#include <string>
#include <irrKlang.h>
#include <map>

namespace BMS
{
namespace Audio
{
	class Sound;

	class AudioManager
	{
	public:
		static AudioManager* instance();

	public:
		AudioManager();

		bool init();
		void shutdown();
		
		Sound* loadSound(const char* filename);
		void clearCache();

	protected:
		static AudioManager* sInstance;

	protected:
		irrklang::ISoundEngine* m_engine;
		std::map<std::string, irrklang::ISoundSource*> m_mediaCache;
	};
}
}