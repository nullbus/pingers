#include "GameWav.h"
#include "../util/Common.h"
#define INITGUID

#pragma warning(disable:4800)
#pragma warning(disable:4996)

GameWav::GameWav()
{
	wavIndex.resize(MAX_NOTE);
	for(unsigned i=0; i<wavIndex.size(); i++)
		wavIndex[i] = new Wave();
}

GameWav::~GameWav()
{
	for(WORD i=0; i<MAX_NOTE; i++)
		delete wavIndex[i];
}

bool GameWav::loadWave(WORD index, DoBmsWav *bmsWav, string& src)
{
	if(index>=MAX_NOTE)
		return false;

	char temp[][4] = {"wav", "ogg", "mp3"};	
	char *cName = new char[src.length()+1];
	strcpy(cName, src.c_str());

	if(!wavIndex[index]->load(bmsWav, string(cName))){

		// 없을경우 확장자교체하고 불러와보기.
		bool bLoad = false;
		for(int i=0; i<3; i++){
			strcpy(strrchr(cName, '.')+1, temp[i]);

			if(wavIndex[index]->load(bmsWav, string(cName))){
				bLoad = true;
				break;
			}
		}

		if(!bLoad){
			return false;
		}
	}
	
	return true;
}

void GameWav::playWave(DoBmsWav *bmsWave, WORD index)
{
	if(index>=MAX_NOTE || wavIndex[index]->bValid == false)
		return;

	wavIndex[index]->play(bmsWave);
}

void GameWav::clear()
{
	for(int i=0; i<MAX_NOTE; i++){
		wavIndex[i]->unload();
	}
}