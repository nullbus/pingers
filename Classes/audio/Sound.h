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
			bool init(irrklang::ISoundEngine* engine, irrklang::ISoundSource* audio);

			Sound();
			virtual ~Sound();

			void play();
			void stop();

		protected:
			// reference
			irrklang::ISoundEngine* m_engine;

			// we manage it
			irrklang::ISoundSource* m_player;
			irrklang::ISound* m_playing;

			// for debug
			std::string m_path;
		};
	}
}