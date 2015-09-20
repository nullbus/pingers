/*
 *	GameWav
 *	bms 내 사운드 관리.
 */

#ifndef __GAMEWAV_H__
#define __GAMEWAV_H__

#include <vector>
#include <string>
#include <windows.h>
#include "DoBmsWav.h"
#include "Wave.h"
using namespace std;

class GameWav  
{
public:
	GameWav();
	~GameWav();

	bool loadWave(WORD index, DoBmsWav *bmsWav, string& src);
	void playWave(DoBmsWav *bmsWave, WORD index);
	void clear();

	vector<Wave*> wavIndex;
};

#endif // #ifndef __GAMEWAV_H__
