#ifndef __CALSCORE_H__
#define __CALSCORE_H__

#include <vector>
#include "define.h"

class CalScore {
public:
	int zhaMa;
	//std::vector<Pai> m_retCardList;
public:
	int getZhaMa() {
		return zhaMa;
	}
	int addZhaMa(Pai pai, int idx, int zhuang) {
		//pai %= 9;
		//pai %= 4;
		//std::cerr << "idx = " << idx << " zhuang = " << zhuang << " pai = " << pai << std::endl;
		if((idx-zhuang+4)%4 == (pai%9)%4) {
			zhaMa ++;
		}
	}
	int init() {
		zhaMa = 0;
		return 0;
	}
	int GetScore(int type, int huIdx, int idx, int zhuang, int dahu) {
		int Score = 0;
		if(type == 0) {
			if(huIdx == zhuang) {
				Score += 1;
			}
			if(idx == zhuang) {
				Score += 1;
			}
			if(dahu) {
				Score += 3;
				while(-- dahu > 0) {
					Score *= 2;
				}
			}
			else {
				Score += 1;
			}
		}
		else if(type == 1) {
			if(huIdx == zhuang) {
				Score += 1;
			}
			if(idx == zhuang) {
				Score += 1;
			}
			if(dahu) {
				Score += 6;
				while(-- dahu > 0) {
					Score *= 2;
				}
			}
			else {
				Score += 1;
			}
		}

		for(int i = 0; i < zhaMa; i ++) {
			Score *= 2;
		}
		return -Score;
	}
};

#endif

