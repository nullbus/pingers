#pragma once

namespace BMS
{
	namespace Event
	{
		class SpeedEvent
		{
		public:
			static const char* CHANGED;

		public:
			SpeedEvent(float speed) : mSpeed(speed) {}
			float speed() const
			{
				return mSpeed;
			}

		protected:
			float mSpeed;
		};
	}
}