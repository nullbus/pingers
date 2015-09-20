#pragma once

#include <audiere.h>
#include <windows.h>
#include <string>

namespace BMS
{
namespace Audio
{
	class AudioManager
	{
	public:
		static AudioManager* instance();

	public:
		AudioManager();

		bool init();
		void shutdown();
		const audiere::AudioDevicePtr& getDevice() const;

	protected:
		static AudioManager* sInstance;

	protected:
		audiere::AudioDevicePtr mDevice;
	};
}
}