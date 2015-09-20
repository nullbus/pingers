#pragma once

#include <windows.h>
#include <string>
#include <vlc/vlc.h>
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
		libvlc_instance_t* m_vlc;
		std::map<std::string, libvlc_media_t*> m_mediaCache;
	};
}
}