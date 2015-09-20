#pragma once

#include <string>
#include "audiere.h"

namespace BMS
{
namespace Audio
{
	class Wave
	{
	public: 
		Wave(const std::string& path);
		~Wave();

		void play();
		void stop();

	protected:
		audiere::OutputStreamPtr m_sound;
		std::string m_path;
	};
}
}