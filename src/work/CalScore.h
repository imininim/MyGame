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
	int GetScore(int type) {
		return 10;
	}
};

#endif

