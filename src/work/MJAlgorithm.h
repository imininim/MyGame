#ifndef __MJALGORITHM_H__
#define __MJALGORITHM_H__

#include "define.h"

class MJAlgorithm {
public:
	
public:
	virtual bool isHu(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isGuo(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isChu(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isGang(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isAnGang(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isPeng(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isChi(const PlayerCards &playerCards, const int type, const Pai pai)=0;
};

//平江麻将
class PingjiangMJ:public MJAlgorithm {
public:
	virtual bool isPeng(const PlayerCards &playerCards, const int type, const Pai pai);

	virtual bool isGang(const PlayerCards &playerCards, const int type, const Pai pai);

	virtual bool isGuo(const PlayerCards &playerCards, const int type, const Pai pai);
	virtual bool isChu(const PlayerCards &playerCards, const int type, const Pai pai);
	virtual bool isChi(const PlayerCards &playerCards, const int type, const Pai pai);
	virtual bool isAnGang(const PlayerCards &playerCards, const int type, const Pai pai);

	virtual bool isHu(const PlayerCards &playerCards, const int type, const Pai pai);
	bool pengPengHu(const PlayerCards &playerCards, const int type, const Pai pai);
	bool jiangJiangHu(const PlayerCards &playerCards, const int type, const Pai pai);
	bool oneColer(const PlayerCards &playerCards, const int type, const Pai pai);
	bool serverDui(const PlayerCards &playerCards, const int type, const Pai pai);

private:
	bool Judge4X3(Pai m_mj[], int ret);
};

class GamePlayWay {
public:
	PingjiangMJ *m_alg;
public:
	GamePlayWay() {
		m_alg = new PingjiangMJ();
	}
public:
	int Operator(const PlayerCards &playerCards, int &Operator, const int type, const Pai pai) {
		int rpai = PlayerCards::reTransCard(pai);
		Operator = 0;
		if(m_alg->isHu(playerCards, type, rpai)) {
			Operator |= HU;
		}
		if(m_alg->isPeng(playerCards, type, rpai)) {
			Operator |= PENG;
		}
		//std::cerr << "OPT" << std::endl;
		if(m_alg->isGang(playerCards, type, rpai)) {
			//std::cerr << "GANG" << std::endl;
			Operator |= GANG;
		}
		if(m_alg->isAnGang(playerCards, type, rpai)) {
			//std::cerr << "ANGANG" << std::endl;
			Operator |= GANG;
		}
		if(m_alg->isGuo(playerCards, type, rpai)) {
			Operator |= GUO;
		}
		//std::cerr << "in Operator" << std::endl;
		if(m_alg->isChu(playerCards, type, rpai)) {
			//std::cerr << "Chu" << std::endl;
			Operator |= CHUPAI;
		}
		if(m_alg->isChi(playerCards, type, rpai)) {
			//std::cerr << "Chu" << std::endl;
			Operator |= CHI;
		}
		return 0;
	}
};

#endif
