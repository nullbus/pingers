#include "Sound.h"
#include "util/logutil.h"

namespace BMS
{
	namespace Audio
	{
		namespace
		{
			//const char* alternative_suffix[] = {"wav", "mp3", "ogg"};
			//const int alternative_length = sizeof(alternative_suffix) / sizeof(alternative_suffix[0]);
		}

		Sound::Sound() : m_player(0) {}

		Sound::~Sound()
		{
			Log::d("delete wave");
			stop();

			if (m_player)
			{
				m_player->drop();
				m_player = NULL;
			}
		}

		bool Sound::init(irrklang::ISound* audio)
		{
			m_player = audio;
			if (m_player)
				m_path = m_player->getSoundSource()->getName();

			return true;
		}

		void Sound::play()
		{
			if (!m_player)
				return;

			stop();

			m_player->setPlayPosition(0);
			m_player->setIsPaused(false);
		}

		void Sound::stop()
		{
			if (!m_player)
				return;

			m_player->stop();
		}
	}
}