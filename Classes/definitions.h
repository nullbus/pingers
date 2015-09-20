#pragma once

#define MAX_NOTE 36*36
#define MAX_KEY 16
#define SINK1	2400.0
#define SINK2	16

#define SAFE_RELEASE(x) if(x){delete (x);(x)=NULL;}

#include "string"
using namespace std;

namespace BMS
{

	enum RANK
	{
		VHARD,
		HARD,
		NORMAL,
		EASY
	};

	enum NUMKEY
	{
		KEY_5	= 6,
		KEY_7	= 8,
		KEY_10	= 12,
		KEY_14	= 16
	};

	/**
	 * 1: Player 1 채널만을 사용한다. (1인 플레이)
	 * 2: Player 1과 2 채널을 모두 사용한다. (2인 플레이)
	 * 3: Player 1과 2 채널을 결합하여 사용한다. (1인 더블 플레이)
	 * 4: Player 1 채널만을 사용하고, Player 2 채널에는 Player 1 채널과 같은 데이터가 들어 있는 것으로 간주한다. (2인 플레이)
	 */
	enum PLAYTYPE
	{
		SINGLE = 1,
		DUET,
		DOUBLE,
		COPY
	};

	enum JUDGE
	{
		JUDGE_KOOL,
		JUDGE_COOL,
		JUDGE_GOOD,
		JUDGE_MISS1,	// 잔미스
		JUDGE_MISS2,	// 진미스
		JUDGE_FAIL
	};

	struct FRECT
	{
		float left;
		float top;
		float right;
		float bottom;
	};
}