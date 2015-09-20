#include "AudioManager.h"
#include <tchar.h>
#include "Sound.h"

namespace BMS
{
	namespace Audio
	{
		AudioManager* AudioManager::sInstance = NULL;

		AudioManager* AudioManager::instance()
		{
			if(NULL == sInstance)
			{
				sInstance = new AudioManager();
				if(!sInstance->init())
				{
					delete sInstance;
					sInstance = NULL;
				}
			}
			return sInstance;
		}

		AudioManager::AudioManager() :
			m_vlc(0)
		{}

		bool AudioManager::init()
		{
			auto vlc = libvlc_new(0, NULL);
			if (!vlc)
				return false;

			return true;
		}

		void AudioManager::shutdown()
		{
			if (m_vlc)
			{
				libvlc_release(m_vlc);
				m_vlc = NULL;
			}
		}

		Sound* AudioManager::loadSound(const char* path)
		{
			if (!m_vlc)
				return NULL;

			auto cachedMedia = m_mediaCache.find(path);
			libvlc_media_t* media = NULL;
			if (cachedMedia != m_mediaCache.end())
			{
				media = cachedMedia->second;
			}
			else
			{
				media = libvlc_media_new_location(m_vlc, path);
				if (!media)
					return NULL;

				m_mediaCache.insert(std::make_pair(path, media));
			}

			auto sound = new Sound;
			if (sound->init(media))
				return sound;

			delete sound;
			return NULL;
		}

		void AudioManager::clearCache()
		{
			for (auto it = m_mediaCache.begin(); it != m_mediaCache.end(); ++it)
			{
				// libvlc_media_new_location할 때 이미 retain되어 있으므로 해제해 주어야 한다
				libvlc_media_release(it->second);
			}

			std::map<std::string, libvlc_media_t*> empty;
			m_mediaCache.swap(empty);
		}
	}
}