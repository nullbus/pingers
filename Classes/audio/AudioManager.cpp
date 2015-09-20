#include "AudioManager.h"
#include <tchar.h>

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
		mDevice(NULL)
	{}

	bool AudioManager::init()
	{
		mDevice = audiere::OpenDevice();
		return mDevice != NULL;
	}

	void AudioManager::shutdown()
	{
		mDevice = NULL;
		delete sInstance;
	}

	const audiere::AudioDevicePtr& AudioManager::getDevice() const
	{
		return mDevice;
	}
}
}