#pragma once


namespace BMS
{

	namespace Key
	{
		enum KeyState
		{
			KEY_UP,
			KEY_DOWN
		};

		enum MappedKey
		{
			Player1_Scratch = 0,
			Player1_1,
				Player1_2,
			Player1_3,
				Player1_4,
			Player1_5,
				Player1_6,
			Player1_7,

			Player2_Scratch,
			Player2_1,
				Player2_2,
			Player2_3,
				Player2_4,
			Player2_5,
				Player2_6,
			Player2_7,

			SpeedUp,
			SpeedDown,
			NUM_KEYS
		};
	}
}