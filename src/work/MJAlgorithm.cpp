#include "MJAlgorithm.h"

//int JIANG[] = {0,1,0,0,1,0,0,1,0, 0,1,0,0,1,0,0,1,0, 0,1,0,0,1,0,0,1,0};

bool PingjiangMJ::isChi(const PlayerCards &playerCards, const int type, const Pai pai) {
	if(type == 0) {
		return false;
	}
	if((pai+2<27) && playerCards.m_playCards[pai+1] > 0 && playerCards.m_playCards[pai+2] > 0 && ((pai+2+8)/9)==((pai+8)/9)) {
		return true;
	}
	if(((pai+1)<27) && (pai>=1) && playerCards.m_playCards[pai+1]>0 && playerCards.m_playCards[pai-1]>0 && ((pai+1+8)/9)==((pai+7)/9)) {
		return true;
	}
	if(pai>=2 && playerCards.m_playCards[pai-2]>0 && playerCards.m_playCards[pai-1]>0 && ((pai+8)/9)==((pai+6)/9)) {
		return true;
	}
	return false;
}

bool PingjiangMJ::isPeng(const PlayerCards &playerCards, const int type, const Pai pai) {
	//如果手上少于两张一样的牌，就不能碰
	if(type == 0) {
		std::cerr << "type = 0" << std::endl;
		return false;
	}
	std::cerr << "m_playCards = " << playerCards.m_playCards[pai] << std::endl;
	if(playerCards.m_playCards[pai] >= 2) {
		return true;
	}
	return false;
}

bool PingjiangMJ::isGang(const PlayerCards &playerCards, const int type, const Pai pai) {
	//如果别人打的
	if(type == 1) {
		if(playerCards.m_playCards[pai] == 3) {
			std::cerr << "peng gang" << std::endl;
			return true;
		}
	}
	else if(type == 0) {
		for(int i = 0; i < playerCards.m_playCardsPeng.size(); i ++) {
			if(playerCards.m_playCardsPeng[i][0] == pai) {
				std::cerr << "mo gang" << std::endl;
				return true;
			}
		}
	}
	return false;
}

bool PingjiangMJ::isGuo(const PlayerCards &playerCards, const int type, const Pai pai) {
	if(type == 0) {
		return false;
	}
	if(playerCards.m_size%3 == 1) {
		return true;
	}
	return false;
}

bool PingjiangMJ::isChu(const PlayerCards &playerCards, const int type, const Pai pai) {
	std::cerr << "type = " << type << std::endl;
	if(type == 1) {
		return false;
	}
	std::cerr << "size = " << playerCards.m_size << std::endl;
	if(playerCards.m_size%3 == 2) {
		return true;
	}
	return false;
}

bool PingjiangMJ::isAnGang(const PlayerCards &playerCards, const int type, const Pai pai) {
	if(type == 0) {
		//std::cerr << "agnang size = " << playerCards.m_playCards[pai] << std::endl;
		for(int i = 0; i < 27; i ++) {
			if(playerCards.m_playCards[i] == 4) {
				return true;
			}
		}
		//if(playerCards.m_playCards[pai] == 4) {
		//	std::cerr << "angang" << std::endl;
		//	return true;
		//}
	}
	return false;
}

bool PingjiangMJ::isHu(const PlayerCards &playerCards, const int type, const Pai pai, int &dahu) {
	int Ret = 0;
	if(pengPengHu(playerCards, type, pai)) {
		std::cerr << "pengPengHu" << std::endl;
		dahu ++;
		Ret = 1;
	}
	if(jiangJiangHu(playerCards, type, pai)) {
		std::cerr << "jiangJiangHu" << std::endl;
		dahu ++;
		Ret = 1;
	}
	if(oneColer(playerCards, type, pai)) {
		std::cerr << "oneColer" << std::endl;
		dahu ++;
		Ret = 1;
	}
	if(serverDui(playerCards, type, pai)) {
		std::cerr << "serverDui" << std::endl;
		dahu ++;
		Ret = 1;
	}

	do {
		if(type == 1) {
			Pai m_mj[30];
			Pai m_mjt[30];
			int ret = 0;
			memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));
			if(JIANG[pai] && m_mjt[pai]>=1) {
				std::cerr << "in jiang hu" << std::endl;
				m_mjt[pai] -= 1;
				ret = 0;
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				ret += playerCards.m_playCardsPeng.size();
				ret += playerCards.m_playCardsGang.size();
				ret += playerCards.m_playCardsAnGang.size();
				if(Judge4X3(m_mj, ret)) {
					std::cerr << "jiang hu" << std::endl;
					Ret = 1;
					break;
				}
			}
			memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));
			if(m_mjt[pai] >= 2) {
				std::cerr << "in no jiang hu" << std::endl;
				m_mjt[pai] -= 2;
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				for(int i = 0; i < 27; i ++) {
					if(JIANG[i]) {
						memcpy(m_mj, m_mjt, sizeof(m_mjt));
						if(m_mj[i] >= 2) {
							ret = 1;
							ret += playerCards.m_playCardsPeng.size();
							ret += playerCards.m_playCardsGang.size();
							ret += playerCards.m_playCardsAnGang.size();
							m_mj[i] -= 2;
							if(Judge4X3(m_mj, ret)) {
								std::cerr << "no jiang hu" << std::endl;
								Ret = 1;
								break;
							}
						}
					}
				}
				if(Ret) {
					break;
				}
			}
			memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));
			if((pai+2 < 27) && m_mjt[pai+1]>0 && m_mjt[pai+2]>0 && (((pai+8)/9) == (pai+10)/9)) {
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				m_mjt[pai+1] -= 1;
				m_mjt[pai+2] -= 1;
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				for(int i = 0; i < 27; i ++) {
					if(JIANG[i]) {
						memcpy(m_mj, m_mjt, sizeof(m_mjt));
						if(m_mj[i] >= 2) {
							ret = 1;
							ret += playerCards.m_playCardsPeng.size();
							ret += playerCards.m_playCardsGang.size();
							ret += playerCards.m_playCardsAnGang.size();
							m_mj[i] -= 2;
							if(Judge4X3(m_mj, ret)) {
								std::cerr << " chi hu 1" << std::endl;
								Ret = 1;
								break;
							}
						}
					}
				}
				if(Ret) {
					break;
				}
			}
			memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));
			if((pai-2 >= 0) && m_mjt[pai-1]>0 && m_mjt[pai-2]>0 && (((pai+8)/9) == (pai+6)/9)) {
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				m_mjt[pai-1] -= 1;
				m_mjt[pai-2] -= 1;
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				for(int i = 0; i < 27; i ++) {
					if(JIANG[i]) {
						memcpy(m_mj, m_mjt, sizeof(m_mjt));
						if(m_mj[i] >= 2) {
							ret = 1;
							ret += playerCards.m_playCardsPeng.size();
							ret += playerCards.m_playCardsGang.size();
							ret += playerCards.m_playCardsAnGang.size();
							m_mj[i] -= 2;
							if(Judge4X3(m_mj, ret)) {
								std::cerr << " chi hu 2" << std::endl;
								Ret = 1;
								break;
							}
						}
					}
				}
				if(Ret) {
					break;
				}
			}
			memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));
			if((pai-1 >= 0 && pai+1<27) && m_mjt[pai-1]>0 && m_mjt[pai+1]>0 && (((pai+9)/9) == (pai+7)/9)) {
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				m_mjt[pai-1] -= 1;
				m_mjt[pai+1] -= 1;
				memcpy(m_mj, m_mjt, sizeof(m_mjt));
				for(int i = 0; i < 27; i ++) {
					if(JIANG[i]) {
						memcpy(m_mj, m_mjt, sizeof(m_mjt));
						if(m_mj[i] >= 2) {
							ret = 1;
							ret += playerCards.m_playCardsPeng.size();
							ret += playerCards.m_playCardsGang.size();
							ret += playerCards.m_playCardsAnGang.size();
							m_mj[i] -= 2;
							if(Judge4X3(m_mj, ret)) {
								std::cerr << " chi hu 3" << std::endl;
								Ret = 1;
								break;
							}
						}
					}
				}
				if(Ret) {
					break;
				}
			}
		}
		else {
			//摸到牌时判断是否胡牌
			Pai m_mj[30];
			int ret = 0;
			for(int i = 0; i < 27; i ++) {
				if(JIANG[i]) {
					std::cerr << "jiang = " << i << std::endl;
					memcpy(m_mj, playerCards.m_playCards, sizeof(playerCards.m_playCards));
					ret = 0;
					if(m_mj[i] >= 2) {
						std::cerr << "i = " << i << std::endl;
						std::cerr << "M_mj.size() = " << m_mj[i] << std::endl;
						ret += playerCards.m_playCardsPeng.size();
						ret += playerCards.m_playCardsGang.size();
						ret += playerCards.m_playCardsAnGang.size();
						m_mj[i] -= 2;
						if(Judge4X3(m_mj, ret)) {
							Ret = 1;
							break;
						}
					}
				}
			}
			if(Ret) {
				break;
			}
		}
	} while(0);
	/*if(Ret) {
		if(playerCards.m_playCardsChi.size()) {

		}
	}*/
	if(Ret) {
		return true;
	}
	return false;
}

bool PingjiangMJ::pengPengHu(const PlayerCards &playerCards, const int type, const Pai pai) {
	//先判断是否满足碰碰胡的基本条件
	if(playerCards.m_playCardsChi.size()) {
		return false;
	}

	int ret = 0;
	ret += playerCards.m_playCardsPeng.size();
	ret += playerCards.m_playCardsGang.size();

	Pai m_mj[30];
	memcpy(m_mj, playerCards.m_playCards, sizeof(playerCards.m_playCards));
	if(type == 1) {
		m_mj[pai] ++;
	}

	int hasJiang = 0;
	for(int i = 0; i < 27; i ++) {
		if(m_mj[i] == 3) {
			ret ++;
		}
		if(m_mj[i] == 2 && JIANG[i]) {
			hasJiang = 1;
		}
	}

	if(ret == 4 && hasJiang) {
		return true;
	}
	return false;
}

bool PingjiangMJ::jiangJiangHu(const PlayerCards &playerCards, const int type, const Pai pai) {
	//碰碰胡的基础上才能将将胡
	Pai m_mj[30];
	memcpy(m_mj, playerCards.m_playCards, sizeof(playerCards.m_playCards));
	for(int i = 0; i < 27; i ++) {
		if(m_mj[i] && (JIANG[i]==0)) {
			return false;
		}
	}
	for(int i = 0; i < playerCards.m_playCardsGang.size(); i ++) {
		if(JIANG[playerCards.m_playCardsGang[0][0]] == 0) {
			return false;
		}
	}
	for(int i = 0; i < playerCards.m_playCardsAnGang.size(); i ++) {
		if(JIANG[playerCards.m_playCardsAnGang[0][0]] == 0) {
			return false;
		}
	}
	for(int i = 0; i < playerCards.m_playCardsPeng.size(); i ++) {
		if(JIANG[playerCards.m_playCardsPeng[0][0]] == 0) {
			return false;
		}
	}
	if(playerCards.m_playCardsChi.size() != 0) {
		return false;
	}
	return true;
}

bool PingjiangMJ::oneColer(const PlayerCards &playerCards, const int type, const Pai pai) {
	//胡牌的基础上才能清一色
	Pai m_mj[30];
	memcpy(m_mj, playerCards.m_playCards, sizeof(playerCards.m_playCards));
	int flag = 0;
	for(int i = 0; i < 9; i ++) {
		if(m_mj[i]) {
			flag = 1;
		}
	}

	for(int i = 9; i < 18; i ++) {
		if(m_mj[i]) {
			flag |= 2;
		}
	}
	for(int i = 18; i < 27; i ++) {
		if(m_mj[i]) {
			flag |= 4;
		}
	}

	for(int i = 0; i < playerCards.m_playCardsPeng.size(); i ++) {
		if(playerCards.m_playCardsPeng[i][0] < 9) {
			flag |= 1;
		}
		else if(playerCards.m_playCardsPeng[i][0] < 18) {
			flag |= 2;
		}
		else if(playerCards.m_playCardsPeng[i][0] < 27) {
			flag |= 4;
		}
	}
	for(int i = 0; i < playerCards.m_playCardsGang.size(); i ++) {
		if(playerCards.m_playCardsGang[i][0] < 9) {
			flag |= 1;
		}
		else if(playerCards.m_playCardsGang[i][0] < 18) {
			flag |= 2;
		}
		else if(playerCards.m_playCardsGang[i][0] < 27) {
			flag |= 4;
		}
	}

	for(int i = 0; i < playerCards.m_playCardsAnGang.size(); i ++) {
		if(playerCards.m_playCardsAnGang[i][0] < 9) {
			flag |= 1;
		}
		else if(playerCards.m_playCardsAnGang[i][0] < 18) {
			flag |= 2;
		}
		else if(playerCards.m_playCardsAnGang[i][0] < 27) {
			flag |= 4;
		}
	}

	for(int i = 0; i < playerCards.m_playCardsChi.size(); i ++) {
		if(playerCards.m_playCardsChi[i][0] < 9) {
			flag |= 1;
		}
		else if(playerCards.m_playCardsChi[i][0] < 18) {
			flag |= 2;
		}
		else if(playerCards.m_playCardsChi[i][0] < 27) {
			flag |= 4;
		}
	}

	if(flag!=1 && flag!=2 && flag!=4) {
		return false;
	}

	Pai m_mjt[30];
	memcpy(m_mjt, playerCards.m_playCards, sizeof(playerCards.m_playCards));

	if(type == 1) {
		m_mjt[pai] ++;
	}

	int ret = 0;
	for(int i = 0; i < 27; i ++) {
		if(JIANG[i]) {
			memcpy(m_mj, m_mjt, sizeof(m_mjt));
			ret = 0;
			if(m_mj[i] >= 2) {
				ret += playerCards.m_playCardsPeng.size();
				ret += playerCards.m_playCardsGang.size();
				ret += playerCards.m_playCardsAnGang.size();
				ret += playerCards.m_playCardsChi.size();
				m_mj[i] -= 2;
				if(Judge4X3(m_mj, ret)) {
					return true;
				}
			}
		}
	}

	return false;
}

bool PingjiangMJ::serverDui(const PlayerCards &playerCards, const int type, const Pai pai) {
	Pai m_mj[30];
	memcpy(m_mj, playerCards.m_playCards, sizeof(playerCards.m_playCards));
	if(type == 1) {
		m_mj[pai] ++;
	}
	int ret = 0;
	for(int i = 0; i < 27; i ++) {
		while(m_mj[i] >= 2) {
			m_mj[i] -= 2;
			ret ++;
		}
	}
	if(ret == 7) {
		return true;
	}
	return false;
}

/*bool xiaoHu() {
	int flag = 0;
	for(int i = 0; i < m_JIANG.size(); i ++) {
		if(m_mjPrivateCal[m_JIANG[i]]) {
			flag = 1;
		}
	}
	if(gang()) {
		
	}
}*/

	/*bool hu() {
		//砰砰胡 ok
		//将将胡
		//清一色
		//七小对
		//小胡
		//平胡
		if(Judge1()) {

		}
	}*/

bool PingjiangMJ::Judge4X3(Pai m_mj[], int ret) {
	//枚举每种花色
	for(int i = 0; i <= 18; i += 9) {
		//枚举是否可以组成3个
		for(int j = 0; j < 9; j ++) {
			if(m_mj[i+j] >= 3) {
				m_mj[i+j] -= 3;
				ret ++;
			}
			while(j+2<9 && m_mj[i+j] && m_mj[i+j+1] && m_mj[i+j+2]) {
				m_mj[i+j] --;
				m_mj[i+j+1] --;
				m_mj[i+j+2] --;
				ret ++;
			}
		}
	}
	if(ret != 4) {
		return false;
	}
	return true;
}
