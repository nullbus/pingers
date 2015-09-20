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
				libvlc_media_player_release(m_player);
				m_player = NULL;
			}
		}

		bool Sound::init(libvlc_media_t* audio)
		{
			auto player = libvlc_media_player_new_from_media(audio);
			if (!player)
				return false;

			m_player = player;
			return true;
		}

		void Sound::play()
		{
			if (!m_player)
				return;

			stop();

			libvlc_media_player_set_position(m_player, .0f);
			libvlc_media_player_play(m_player);
		}

		void Sound::stop()
		{
			if (!m_player)
				return;

			libvlc_media_player_stop(m_player);
		}
	}
}