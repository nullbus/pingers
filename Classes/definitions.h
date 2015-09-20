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
	 * 1: Player 1 ä�θ��� ����Ѵ�. (1�� �÷���)
	 * 2: Player 1�� 2 ä���� ��� ����Ѵ�. (2�� �÷���)
	 * 3: Player 1�� 2 ä���� �����Ͽ� ����Ѵ�. (1�� ���� �÷���)
	 * 4: Player 1 ä�θ��� ����ϰ�, Player 2 ä�ο��� Player 1 ä�ΰ� ���� �����Ͱ� ��� �ִ� ������ �����Ѵ�. (2�� �÷���)
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
		JUDGE_MISS1,	// �ܹ̽�
		JUDGE_MISS2,	// ���̽�
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