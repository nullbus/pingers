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

		Sound::Sound() : m_player(0), m_engine(0), m_playing(0) {}

		Sound::~Sound()
		{
			Log::d("delete wave");

			stop();

			// do not drop source and engine here
			m_player = NULL;
			m_engine = NULL;
		}

		bool Sound::init(irrklang::ISoundEngine* engine, irrklang::ISoundSource* audio)
		{
			m_engine = engine;
			m_player = audio;
			if (m_player)
				m_path = audio->getName();

			return true;
		}

		void Sound::play()
		{
			if (!m_player)
				return;

			stop();

			m_playing = m_engine->play2D(m_player);
		}

		void Sound::stop()
		{
			if (!m_playing)
				return;

			m_playing->stop();
			m_playing->drop();
			m_playing = NULL;
		}
	}
}