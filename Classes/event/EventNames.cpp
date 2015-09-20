#include "BarScaleEvent.h"
#include "BpmEvent.h"
#include "ComboEvent.h"
#include "GameConfigEvent.h"
#include "JudgeEvent.h"
#include "KeyEvent.h"
#include "SpeedEvent.h"

namespace BMS
{
namespace Event
{
	const char* KeyEvent::UP = "KeyEvent::UP";
	const char* KeyEvent::DOWN = "KeyEvent::DOWN";

	const char* BpmEvent::CHANGED = "BpmEvent::CHANGED";

	const char* SpeedEvent::CHANGED = "SpeedEvent::CHANGED";

	const char* BarScaleEvent::CHANGED = "BarScaleEvent::CHANGED";

	const char* ComboEvent::CHANGED = "ComboEvent::CHANGED";

	const char* JudgeEvent::JUDGE = "JudgeEvent::JUDGE";

	const char* GameConfigEvent::RESET = "GameConfigEvent::RESET";
}
}