#ifndef __MJALGORITHM_H__
#define __MJALGORITHM_H__

#include "define.h"

const int JIANG[] = {0,1,0,0,1,0,0,1,0, 0,1,0,0,1,0,0,1,0, 0,1,0,0,1,0,0,1,0};

class MJAlgorithm {
public:
	
public:
	virtual bool isHu(const PlayerCards &playerCards, const int type, const Pai pai, int &dahu)=0;
	virtual bool isGuo(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isChu(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isGang(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isAnGang(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isPeng(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isChi(const PlayerCards &playerCards, const int type, const Pai pai)=0;
	virtual bool isTianhu(const PlayerCards &playerCards, const int type, const Pai pai)=0;
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
	virtual bool isTianhu(const PlayerCards &playerCards, const int type, const Pai pai);

	virtual bool isHu(const PlayerCards &playerCards, const int type, const Pai pai, int &hu);
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
	int isTingPai(const PlayerCards &playerCards, int &Operator, const int type, const Pai pai) {
		if(type) {
			for(int i = 0; i < 27; i ++) {
				PlayerCards tmp = playerCards;
				tmp.addCard(i);
				int dahu;
				std::cerr << "play size = " << tmp.m_size << std::endl;
				if(m_alg->isHu(tmp, 0, 0, dahu)) {
					Operator |= HU;
					return 0;
				}
			}
		}
		return 0;
	}
	int Operator(const PlayerCards &playerCards, int &Operator, const int type, const Pai pai, int &dahu) {
		int rpai = PlayerCards::reTransCard(pai);
		std::cerr << "rpai = " << rpai << std::endl;
		std::cerr << "pai = " << pai << std::endl;
		Operator = 0;
		if(m_alg->isTianhu(playerCards, type, rpai)) {
			Operator |= TIANHU;
		}
		if(m_alg->isHu(playerCards, type, rpai, dahu)) {
			Operator |= HU;
		}
		if(m_alg->isPeng(playerCards, type, rpai)) {
			Operator |= PENG;
		}
		//std::cerr << "OPT" << std::endl;
		if(type == 0) {
			for(int i = 0; i < 27; i ++) {
				if(playerCards.m_playCards[i] > 0) {
					if(m_alg->isGang(playerCards, type, i)) {
						Operator |= GANG;
					}
				}
			}
		}
		else if(m_alg->isGang(playerCards, type, rpai)) {
			//std::cerr << "GANG" << std::endl;
			Operator |= GANG;
		}
		if(m_alg->isAnGang(playerCards, type, rpai)) {
			//std::cerr << "ANGANG" << std::endl;
			Operator |= ANGANG;
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
