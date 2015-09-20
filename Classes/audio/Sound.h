#pragma once

#include <string>
#include <vlc/vlc.h>

namespace BMS
{
	namespace Audio
	{
		class Sound
		{
		public: 
			bool init(libvlc_media_t* audio);

			Sound();
			virtual ~Sound();

			void play();
			void stop();

		protected:
			libvlc_media_player_t* m_player;
			std::string m_path;
		};
	}
}