#include "Wave.h"
#include "AudioManager.h"
#include "util/logutil.h"

namespace BMS
{
namespace Audio
{
	namespace
	{
		const char* alternative_suffix[] = {"wav", "mp3", "ogg"};
		const int alternative_length = sizeof(alternative_suffix) / sizeof(alternative_suffix[0]);
	}

	Wave::Wave(const std::string& path) : m_sound(NULL)
	{
		std::string postfix = m_path = path;

		m_sound = OpenSound(AudioManager::instance()->getDevice(), m_path.c_str(), false);
		if (m_sound.get())
			return;

		// if unsuccessful, find alternative path
		unsigned dotPos = postfix.find_last_of('.');
		if(dotPos != std::string::npos)
			postfix = postfix.substr(0, dotPos+1);

		for(int i=0; i<alternative_length; i++)
		{
			std::string fullpath = postfix + alternative_suffix[i];
			m_sound = OpenSound(AudioManager::instance()->getDevice(), fullpath.c_str(), false);
			if (m_sound.get())
				break;
		}
	}

	Wave::~Wave()
	{
		Log::d("delete wave");
		stop();
		m_sound = NULL;
	}

	void Wave::play()
	{
		stop();
		if(m_sound.get())
		{
			m_sound->play();
		}
	}

	void Wave::stop()
	{
		if(m_sound.get())
		{
			m_sound->stop();
			m_sound->reset();
		}
	}
}
}