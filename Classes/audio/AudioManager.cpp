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

		AudioManager::AudioManager() : m_engine(0)
		{
		}

		bool AudioManager::init()
		{
			m_engine = irrklang::createIrrKlangDevice();
			if (!m_engine)
				return false;

			return true;
		}

		void AudioManager::shutdown()
		{
			if (m_engine)
			{
				m_engine->drop();
				m_engine = NULL;
			}
		}

		Sound* AudioManager::loadSound(const char* path)
		{
			if (!m_engine)
				return NULL;

			auto cachedMedia = m_mediaCache.find(path);
			irrklang::ISoundSource* media = NULL;
			if (cachedMedia != m_mediaCache.end())
			{
				media = cachedMedia->second;
			}
			else
			{
				media = m_engine->addSoundSourceFromFile(path, irrklang::ESM_AUTO_DETECT, true);
				if (!media)
					return NULL;

				m_mediaCache.insert(std::make_pair(path, media));
			}

			auto sound = new Sound;
			if (sound->init(m_engine, media))
				return sound;

			delete sound;
			return NULL;
		}

		void AudioManager::clearCache()
		{
			for (auto it = m_mediaCache.begin(); it != m_mediaCache.end(); ++it)
			{
				m_engine->removeSoundSource(it->first.c_str());
			}

			std::map<std::string, irrklang::ISoundSource*> empty;
			m_mediaCache.swap(empty);
		}
	}
}