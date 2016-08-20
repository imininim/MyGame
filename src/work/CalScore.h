#ifndef __CALSCORE_H__
#define __CALSCORE_H__

#include <vector>
#include "define.h"

class CalScore {
public:
	std::vector<Pai> m_retCardList;
public:
	int setCard(const std::vector<Pai> &retCardList) {
		m_retCardList.clear();
		m_retCardList = retCardList;
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

		return -Score;
	}
};

#endif

