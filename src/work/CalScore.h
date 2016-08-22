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
		if((idx-zhuang+4)%4 == pai%4) {
			zhaMa ++;
		}
		//if((pai-zhuang)%4 == idx) {
		//	zhaMa ++;
		//}
	}
	int init() {
		//m_retCardList.clear();
		//m_retCardList = retCardList;
		zhaMa = 0;
		return 0;
	}
	int GetScore(int type, int huIdx, int idx, int zhuang, int dahu) {
		int Score = 0;
		if(type == 0) {
			if(dahu) {
				Score += 3*dahu;
			}
			else {
				Score += 1;
			}
			if(huIdx == zhuang) {
				Score += 1;
			}
			if(idx == zhuang) {
				Score += 1;
			}
		}
		else if(type == 1) {
			if(dahu) {
				Score += 6*dahu;
			}
			else {
				Score += 1;
			}
			if(huIdx == zhuang) {
				Score += 1;
			}
			if(idx == zhuang) {
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

