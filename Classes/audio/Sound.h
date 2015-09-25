#pragma once

#include <string>
#include <irrKlang.h>

namespace BMS
{
	namespace Audio
	{
		class Sound
		{
		public: 
			bool init(irrklang::ISound* audio);

			Sound();
			virtual ~Sound();

			void play();
			void stop();

		protected:
			irrklang::ISound* m_player;
			std::string m_path;
		};
	}
}