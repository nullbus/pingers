#pragma once

namespace BMS
{
	namespace Event
	{
		class BarScaleEvent
		{
		public:
			static const char* CHANGED;

		public:
			BarScaleEvent(float scale) : mScale(scale) {}
			float scale() const
			{
				return mScale;
			}

		protected:
			float mScale;
		};
	}
}